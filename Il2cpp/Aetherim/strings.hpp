#pragma once

#include <cstring>

#include "./utf8.h"
#include "./api.hpp"

struct Il2cppString : Il2cppHeader::Il2CppString
{
    auto getRawChar() const -> const char16_t *
    {
        return this->chars;
    }

    auto getString() const -> std::string
    {
        std::string utf8String;
        std::u16string utf16String(this->chars, 0, this->length);

        utf8String.reserve(utf16String.length());
        utf8::unchecked::utf16to8(utf16String.begin(), utf16String.end(), std::back_inserter(utf8String));
        return utf8String;
    }

    auto getChar() const -> const char *
    {
        return (const char *)this->getString().c_str();
    }
};

void *createIl2cppString(const char *str)
{
    if (Il2cppApi::create_string == nullptr)
        return nullptr;

    return (void *)Il2cppApi::create_string(str);
}