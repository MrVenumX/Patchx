#pragma once

#if defined(_WIN64) || defined(_WIN32)
#include <Windows.h>
#endif

#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <vector>

#include "./api.hpp"

struct Il2cppMethod : Il2cppHeader::MethodInfo
{
public:
  Il2cppMethod() = default;

  /**
   * @brief Invoke the method as if it were static.
   */
  template <typename T = void *, typename... Args>
  auto invokeStatic(Args... args) const -> T
  {
    if (this == nullptr)
      return T();

    const T result = (((T(*)(Args...))this->methodPointer)(args...));

    return result;
  }

  /**
   * @brief Invoke the method as if it weren't static.
   */
  template <typename T = void *, typename... Args>
  auto invoke(void *obj, Args... args) -> T
  {
    if (this == nullptr)
      return T();

    const T result = (((T(*)(void *, Args...))this->methodPointer)(obj, args...));

    return result;
  }

  /**
   * @brief Returns the number of parameters the method has.
   */
  auto getParamCount() const -> uint32_t
  {
    if (Il2cppApi::get_method_param_count == nullptr || this == nullptr)
      return -1;

    return Il2cppApi::get_method_param_count(this);
  }

  /**
   * @brief Get the name of a parameter at a specific index.
   */
  auto getParamName(int index) const -> const char *
  {
    if (Il2cppApi::get_method_param_name == nullptr || this == nullptr)
      return "Method->get_param_name() is nullptr";

    return Il2cppApi::get_method_param_name(this, index);
  }

  /**
   * @brief Get the data type of a parameter.
   */
  auto getParamType(int index) const -> const char *
  {
    if (Il2cppApi::get_method_param_type == nullptr || this == nullptr)
      return "Method->get_param_type() is nullptr";

    return Il2cppApi::get_method_param_type(this, index);
  }

  auto isGeneric() const -> bool
  {
    if (Il2cppApi::get_method_is_generic == nullptr || this == nullptr)
      return false;

    return Il2cppApi::get_method_is_generic(this);
  }

  auto isInflated() const -> bool
  {
    if (Il2cppApi::get_method_is_inflated == nullptr || this == nullptr)
      return false;

    return Il2cppApi::get_method_is_inflated(this);
  }

  auto isInstance() const -> bool
  {
    if (Il2cppApi::get_method_is_instance == nullptr || this == nullptr)
      return false;

    return Il2cppApi::get_method_is_instance(this);
  }

  /**
   * @brief Get the method name.
   */
  auto getName() const -> const char *
  {
    if (Il2cppApi::get_method_name == nullptr)
      return "Il2cppMethod->get_name() is nullptr";

    return Il2cppApi::get_method_name(this);
  }
};