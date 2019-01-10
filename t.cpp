
#include <cstdio>

#include "fixnum.hpp"
#include "float.hpp"
#include "int.hpp"
#include "nil.hpp"
#include "double.hpp"

type_id constexpr T::type() const noexcept {
    switch (bits >> 48) {
    case impl::float_tag >> 48:
        return float_id;
    case impl::rune_tag >> 48:
        return rune_id;
    case (impl::fixnum_tag >> 48) + 0:
    case (impl::fixnum_tag >> 48) + 1:
    case (impl::fixnum_tag >> 48) + 2:
    case (impl::fixnum_tag >> 48) + 3:
        return fixnum_id;
    }

    uint64_t addr52 = 0;

    if ((bits >> 52) == (impl::pointer_tag >> 52) &&
	(addr52 = (bits & ~impl::pointer_mask)) != 0 && /* skip +inf */
        addr52 != (uint64_t(1) << 51)) /* skip +NaN */ {

        switch (bits & 0xF) {
        case impl::struct_tag & 0xF:
            return struct_id;
        case impl::cons_tag & 0xF:
            return cons_id;
        case impl::symbol_tag & 0xF:
            return symbol_id;
        case impl::func_tag & 0xF:
            return func_id;
        default:
            return unknown_id;
        }
    }
    return double_id;
}

void T::print(FILE *out) const {
    switch (type()) {
    case double_id:
        fprintf(out, "%f", dbl);
        break;
    case fixnum_id:
        fprintf(out, "%ld", Fixnum::untag(bits));
        break;
    case float_id:
        fprintf(out, "%f", double(fl));
        break;
    case rune_id:
        // FIXME print Unicode
        fprintf(out, "%c", char(i));
        break;
    case struct_id:
        fputs("struct", out);
        break;
    case cons_id:
        fputs(bits == impl::nil_bits ? "nil" : "cons", out);
        break;
    case symbol_id:
        fputs(bits == impl::t_bits ? "t" : "symbol", out);
        break;
    case func_id:
        fputs("func", out);
        break;
    case unknown_id:
    default:
        fputs("unknown", out);
        break;
    }
}
