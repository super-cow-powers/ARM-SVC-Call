/** @file gSvc.h
 * @brief GNU-friendly Service dispatch macros.
 * These are experimental, so expect bugs and that.
 * I've not found any though.
 */
#ifndef GSVC_H
#define GSVC_H

/** @brief Detects if we're compiling with a standard compiler or the old
  ARMCC. Defined if we're using anything but the old ARMCC.
*/
#if (!defined(__ARMCC_VERSION) || __ARMCC_VERSION > 6000000)
#define NOT_ARMCC 1
#else
#define IS_ARMCC 1
#endif

#if defined(__arm__)

/**********************/
/* Service Call Stuff */
/**********************/
#define gnuSVC0(svcNum)                                                       \
  ({                                                                          \
    volatile register uint32_t r0 asm("r0");                                  \
    __asm volatile("SVC %[num]"                                               \
                   : "=r"(r0)                                                 \
                   : [num] "I"(svcNum), "r"(r0)                               \
                   : "memory");                                               \
    r0;                                                                       \
  })
#define gnuSVC1(svcNum, arg)                                                  \
  ({                                                                          \
    volatile register uint32_t r0 asm("r0") = (uint32_t)(arg);                \
    __asm volatile("SVC %[num]"                                               \
                   : "=r"(r0)                                                 \
                   : [num] "I"(svcNum), "r"(r0)                               \
                   : "memory");                                               \
    r0;                                                                       \
  })
#define gnuSVC2(svcNum, arg0, arg1)                                           \
  ({                                                                          \
    volatile register uint32_t r0 asm("r0") = (uint32_t)(arg0);               \
    volatile register uint32_t r1 asm("r1") = (uint32_t)(arg1);               \
    __asm volatile("SVC %[num]"                                               \
                   : "=r"(r0)                                                 \
                   : [num] "I"(svcNum), "r"(r0), "r"(r1)                      \
                   : "memory");                                               \
    r0;                                                                       \
  })
#define gnuSVC3(svcNum, arg0, arg1, arg2)                                     \
  ({                                                                          \
    volatile register uint32_t r0 asm("r0") = (uint32_t)(arg0);               \
    volatile register uint32_t r1 asm("r1") = (uint32_t)(arg1);               \
    volatile register uint32_t r2 asm("r2") = (uint32_t)(arg2);               \
    __asm volatile("SVC %[num]"                                               \
                   : "=r"(r0)                                                 \
                   : [num] "I"(svcNum), "r"(r0), "r"(r1), "r"(r2)             \
                   : "memory");                                               \
    r0;                                                                       \
  })
#define gnuSVC4(svcNum, arg0, arg1, arg2, arg3)                               \
  ({                                                                          \
    volatile register uint32_t r0 asm("r0") = (uint32_t)(arg0);               \
    volatile register uint32_t r1 asm("r1") = (uint32_t)(arg1);               \
    volatile register uint32_t r2 asm("r2") = (uint32_t)(arg2);               \
    volatile register uint32_t r3 asm("r3") = (uint32_t)(arg3);               \
    __asm volatile("SVC %[num]"                                               \
                   : "=r"(r0)                                                 \
                   : [num] "I"(svcNum), "r"(r0), "r"(r1), "r"(r2), "r"(r3)    \
                   : "memory");                                               \
    r0;                                                                       \
  })

#else
#warning "Unknown"
#endif
#define gnuResolveSVC(num, _1, _2, _3, _4, NAME, ...) NAME

/** @brief Issue a Service Call (svc) instruction with upto 4 Arguments.
 * Capable of returning a value from wherever your Arch puts returns.
 * @param svcNum Service Handler Number.
 * @param ... Four or less arguments.
 */

#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 202301L)
#define gnuSVC(svcNum, ...)                                                   \
  gnuResolveSVC (svcNum, ##__VA_ARGS__, gnuSVC4, gnuSVC3, gnuSVC2, gnuSVC1,   \
                 gnuSVC0) (svcNum, ##__VA_ARGS__)
#else // We get __VA_OPT__ in C23
#define gnuSVC(svcNum, ...)                                                   \
  gnuResolveSVC (svcNum __VA_OPT__ (, ) __VA_ARGS__, gnuSVC4, gnuSVC3,        \
                 gnuSVC2, gnuSVC1,                                            \
                 gnuSVC0) (svcNum __VA_OPT__ (, ) __VA_ARGS__)
#endif
#ifdef NOT_ARMCC
/** @brief SVC Keyword should not be used with GNU-flavoured compilers. Define
 * it to prevent mystery issues and produce a known one. Ideally I'd make it
 * issue a compile error if used, but I can't figure it out. This'll do the
 * trick anyway.
 */
#define __svc(ARG) svcerrorsvcerrorsvcerror_ARG
#endif

#endif
