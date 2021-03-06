
#include <cstring>    // std::memset()
#include <new>        // std::bad_alloc
#include <stdexcept>  // std::overflow_error, std::runtime_error
#include <string>     // std::string
#include <sys/mman.h> // mmap()
#include <typeinfo>   // std::bad_cast

#include "nil.hpp"
#include "string.hpp"
#include "symbol.hpp"

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

    /* fixed memory address reserved for nil, t, empty string */
    void * addr = mmap((void *)fixed_addr_bits, 32768,
                       PROT_READ|PROT_WRITE, MAP_FIXED|MAP_PRIVATE|MAP_ANONYMOUS,
                       -1, 0);
    if (addr == MAP_FAILED)
        throw_bad_alloc();

    // initialize nil
    Pair n = nil;
    n->first = n->rest = nil;

    // initialize empty string
    String str;
    impl::string* s = reinterpret_cast<impl::string*>(str.addr());
    s->size = Long{0};
    // std::memset(s->data, '\0', sizeof(Long)); // unnecessary, mmap() zeroes data
    
    // initialize t
    Symbol sym = t;
    const_cast<String &>(sym->name) = String{"t"};
    const_cast<T &>(sym->value) = t;

    return true;
}


void * impl::alloc(size_t bytes) /* throw(std::bad_alloc) */ {
    void * addr = GC_MALLOC(bytes);
    if (!addr) {
        throw_bad_alloc();
    }
    return addr;
}

CRYSP_NORETURN void impl::throw_bad_alloc() /* throw(std::bad_alloc) */ {
    throw std::bad_alloc{};
}

CRYSP_NORETURN void impl::throw_bad_cast() /* throw(std::bad_cast) */ {
    throw std::bad_cast{};
}

CRYSP_NORETURN void impl::throw_out_of_range(const char * message) /* throw(std::bad_cast) */ {
   throw std::out_of_range{message};
}

CRYSP_NORETURN void impl::throw_overflow_error(const char * type_name) /* throw(std::overflow_error) */ {
    const std::string prefix = "integer too large, overflows ";
    throw std::overflow_error{prefix + type_name};
}

CRYSP_NORETURN void impl::throw_runtime_error(const char * message)  /* throw(std::runtime_error) */ {
   throw std::runtime_error{message};
}

CRYSP_NS_END
