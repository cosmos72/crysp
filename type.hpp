#ifndef CRYSP_TYPE_HPP
#define CRYSP_TYPE_HPP

#include "t.hpp"

class Type {
private:
    type_id id_;
    
public:
    inline constexpr Type(type_id id) noexcept : id_(id) {
    }

    template<class E>
    inline constexpr Type() noexcept : id_(E::static_type) {
    }
    
    inline constexpr type_id id() const noexcept {
        return id_;
    }

    const char * name() const noexcept;
};

inline Type type_of(T arg) {
    return Type(arg.type());
}

#endif // CRYSP_TYPE_HPP
