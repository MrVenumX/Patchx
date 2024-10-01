#pragma once

#if defined(_WIN64) || defined(_WIN32)
#include <Windows.h>
#else
#include <string.h>
#endif

#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <vector>

#include "./api.hpp"
#include "./method.hpp"
#include "./field.hpp"

struct Il2cppClass : Il2cppHeader::Il2CppClass
{
private:
  // const char * name {};

public:
  Il2cppClass() = default;

  using fields_t = std::vector<Il2cppField *>;

  auto createNewInstance() const -> Il2cppHeader::Il2CppObject *
  {
    if (Il2cppApi::object_create == nullptr)
      return nullptr;

    return Il2cppApi::object_create(this);
  }

  /**
   * Returns a pointer to the specified method.
   */
  auto getMethod(const char *name, int argc = 0) const -> Il2cppMethod *
  {
    if (Il2cppApi::get_method == nullptr)
      return nullptr;

    auto method = Il2cppApi::get_method(this, name, argc);
    if (!method)
      return nullptr;

    return reinterpret_cast<Il2cppMethod *>(method);
  }

  /**
   * Returns a field given a name.
   */
  auto getField(const char *name) const -> Il2cppField *
  {
    if (Il2cppApi::get_field == nullptr)
      return nullptr;

    auto field = Il2cppApi::get_field(this, name);
    if (!field)
      return nullptr;

    return reinterpret_cast<Il2cppField *>(field);
  }

  /**
   * Returns all fields of the class as a vector of void pointers.
   */
  auto getFields() const -> fields_t
  {
    fields_t m_fields{};
    if (m_fields.size())
      return m_fields;

    if (Il2cppApi::get_field_count == nullptr || Il2cppApi::get_fields == nullptr)
      return m_fields;

    const size_t count = Il2cppApi::get_field_count(this);
    if (count)
      m_fields.resize(count);

    void *iter = NULL;
    void *field = nullptr;

    size_t index = 0;

    while (field = Il2cppApi::get_fields(this, &iter))
    {
      if (!field || field == NULL)
        continue;

      m_fields[index++] = reinterpret_cast<Il2cppField *>(field);
    }

    return m_fields;
  }

  /**
   * Given a name, returns a class that's nested inside of the current class.
   */
  auto getNestedClass(const char *name) const -> Il2cppClass *
  {
    if (Il2cppApi::get_nested_types == nullptr || Il2cppApi::get_class_name == nullptr)
      return nullptr;

    void *iter = NULL;

    while (auto type = Il2cppApi::get_nested_types(this, &iter))
    {
      const auto class_name = Il2cppApi::get_class_name(type);

      if (!strcmp(class_name, name))
        return type;
    }

    return nullptr;
  }

  /**
   * Return the name of the current class.
   */
  auto getName() const -> const char *
  {
    if (Il2cppApi::get_class_name == nullptr)
      return "Il2cppClass->get_name() is nullptr";

    return Il2cppApi::get_class_name(this);
  }
};

struct Il2cppObject
{
  union
  {
    Il2cppClass *klass;
    Il2cppHeader::Il2CppVTable *vtable;
  };
  Il2cppHeader::MonitorData *monitor;
};