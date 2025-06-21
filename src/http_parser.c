#include "http_parser.h"

#include <string.h>

static const char tchar_symbol_table[256] = {
    ['!'] = 1,  ['#'] = 1, ['$'] = 1, ['%'] = 1, ['&'] = 1, ['*'] = 1, ['+'] = 1,
    ['-'] = 1,  ['.'] = 1, ['^'] = 1, ['_'] = 1, ['`'] = 1, ['|'] = 1, ['~'] = 1,
    ['\''] = 1,  // single quote
};

static inline bool is_alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

static inline bool is_digit(char c) { return c >= '0' && c <= '9'; }

static inline bool is_other_tchar(char c) { return tchar_symbol_table[(unsigned char)c]; }

bool is_tchar(char c) { return is_alpha(c) || is_digit(c) || is_other_tchar(c); }
