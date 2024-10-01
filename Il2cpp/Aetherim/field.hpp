#pragma once

#if defined(_WIN64) || defined(_WIN32)
#include <Windows.h>
#endif

#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <vector>

#include "./api.hpp"
#include "./strings.hpp"

struct Il2cppField : Il2cppHeader::FieldInfo
{
public:
  Il2cppField() = default;

  /**
   * Return a static field given a name.
   */
  template <typename T>
  auto getStaticValue() const -> T
  {
    if (this == nullptr)
      return T();

    void *val = NULL;

    Il2cppApi::get_static_field_value(this, &val);

    return (T)val;
  }

  template <typename T>
  auto getValue(const void *obj) const -> T
  {
    if (this == nullptr)
      return T();

    T val = *(T *)((uintptr_t)obj + this->offset);

    return val;
  }

  // Sets the value of a field
  template <typename T>
  auto setValue(const void *obj, T value) const -> void *
  {
    if (this == nullptr)
      return nullptr;

    if ((*(void **)((uintptr_t)obj + this->offset)) == nullptr)
      return nullptr;

    *(T *)((uintptr_t)obj + this->offset) = value;

    return *(void **)((uintptr_t)obj + this->offset);
  }

  /**
   * Sets the value of a static field.
   */
  auto setStaticValue(void *value) const -> void *
  {
    if (Il2cppApi::set_static_field_value == nullptr)
      return nullptr;

    return Il2cppApi::set_static_field_value(this, value);
  }

  /**
   * Returns the offset of a field relative to the class.
   */
  auto getOffset() const -> size_t
  {
    if (Il2cppApi::get_field_offset == nullptr)
      return 0x0;

    return Il2cppApi::get_field_offset(this);
  }

  /**
   * Returns an object pointer of the field.
   */
  auto getObject(const void *obj) const -> void *
  {
    if (Il2cppApi::get_field_object == nullptr)
      return nullptr;

    return Il2cppApi::get_field_object(this, obj);
  }

  /**
   * Returns a pointer to the data type of the field.
   */
  auto getType() const -> const Il2cppHeader::Il2CppType * { return this->type; }

  /**
   * Returns the attribute of the field.
   * Ie. public FIELD, const private FIELD, static FIELD, etc
   */
  auto getAttribute() const -> const char *
  {
    char attr[100];

    auto attrs = this->type->attrs;

    auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
    switch (access)
    {
    case FIELD_ATTRIBUTE_PRIVATE:
      strcpy(attr, "private ");
      break;
    case FIELD_ATTRIBUTE_PUBLIC:
      strcpy(attr, "public ");
      break;
    case FIELD_ATTRIBUTE_FAMILY:
      strcpy(attr, "protected ");
      break;
    case FIELD_ATTRIBUTE_ASSEMBLY:
    case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
      strcpy(attr, "internal ");
      break;
    case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
      strcpy(attr, "protected internal ");
      break;
    }

    if (attrs & FIELD_ATTRIBUTE_LITERAL)
    {
      strcpy(attr, "const ");
    }
    else if (attrs & FIELD_ATTRIBUTE_STATIC)
    {
      strcpy(attr, "static ");
    }
    else if (attrs & FIELD_ATTRIBUTE_INIT_ONLY)
    {
      strcpy(attr, "readonly ");
    }

    return attr;
  }

  /**
   * @brief Get the field name.
   */
  auto getName() const -> const char *
  {
    if (Il2cppApi::get_field_name == nullptr)
      return "Il2cppField->get_name() is nullptr";

    return Il2cppApi::get_field_name(this);
  }
};
