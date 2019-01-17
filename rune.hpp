#ifndef CRYSP_RUNE_HPP
#define CRYSP_RUNE_HPP

#include "t.hpp"

typedef int32_t rune; // Unicode is actually a little less than 21 bits

class Utf8;

class Rune : public T {
private:
    template<class To> friend bool is(T arg);

    static inline constexpr bool typecheck(uint64_t bits) noexcept {
        return (bits >> 48) == (impl::rune_tag >> 48);
    }

public:
    inline constexpr Rune() noexcept
    /**/: T{int32_t(0), uint32_t(impl::rune_tag >> 32)} {
    }

    explicit inline constexpr Rune(rune r) noexcept
        : T{r, uint32_t(impl::rune_tag >> 32)} {
    }

    // defined in utf8.hpp
    explicit inline constexpr Rune(Utf8 utf8) noexcept;

    
    /*
    inline constexpr Rune(const Rune & other) = default;
    inline constexpr Rune & operator=(const Rune & other) = default;
    inline ~Rune() = default;
    */
    
    inline constexpr rune val() const noexcept {
        return i;
    }

    // defined in type.hpp
    inline constexpr Type type() const noexcept;

    inline constexpr type::id type_id() const noexcept {
        return type::rune_id;
    }

    enum {
        static_type_id = type::rune_id,
    };
    
    int print(FILE * out) const;

    Rune & operator=(rune ch) noexcept {
        return (*this) = Rune{ch};
    }
};

#endif // CRYSP_RUNE_HPP
