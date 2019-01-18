
#include <new>        // std::bad_alloc
#include <stdexcept>  // std::overflow_error
#include <string>     // std::string
#include <typeinfo>   // std::bad_cast

#include <sys/mman.h> // mmap()

#include "crysp/int.hpp"
#include "crysp/nil.hpp"

#ifndef GC_MALLOC
#include <cstdlib>   // malloc
#define GC_MALLOC malloc
#endif // GC_MALLOC

CRYSP_NS_START

namespace impl {
    static const bool running = init();
}

bool impl::init() {
    if (running)
        return true;

    /* fixed memory address reserved for nil, t */
    void * addr = mmap((void *)(nil_bits & pointer_unmask), 32768,
                       PROT_READ|PROT_WRITE, MAP_FIXED|MAP_PRIVATE|MAP_ANONYMOUS,
                       -1, 0);
    if (addr == MAP_FAILED)
        throw_bad_alloc();

    Pair x = nil;
    x->first = x->second = nil;
    return true;
}


void * impl::alloc(size_t bytes) /* throw(std::bad_alloc) */ {
    void * addr = GC_MALLOC(bytes);
    if (!addr) {
        throw_bad_alloc();
    }
    return addr;
}

void impl::throw_bad_alloc() /* throw(std::bad_alloc) */ {
    throw std::bad_alloc{};
}

void impl::throw_bad_cast() /* throw(std::bad_cast) */ {
    throw std::bad_cast{};
}

void impl::throw_overflow_error(const char * type_name) /* throw(std::overflow_error) */ {
    const std::string prefix = "integer too large, overflows ";
    throw std::overflow_error{prefix + type_name};
}

CRYSP_NS_END