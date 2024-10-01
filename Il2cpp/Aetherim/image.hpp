#pragma once

#include <functional>
#include <string>
#include <vector>
#include <memory>

#include "./class.hpp"

struct Il2cppImage : Il2cppHeader::Il2CppImage
{
private:
  // const char * m_name {};

public:
  using classes_t = std::vector<Il2cppClass *>;

  /**
   * Returns a pointer to the class given a class name.
   */
  auto getClass(const char *name, const char *namespaze = "") const -> Il2cppClass *
  {
    return Il2cppApi::get_class(this, namespaze, name);
  }

  /**
   * Returns all classes of the image as a vector of void pointers.
   */
  auto getClasses() const -> classes_t
  {
    static classes_t m_classes{};
    if (m_classes.size())
      return m_classes;

    size_t count = Il2cppApi::get_class_count(this);
    if (count)
      m_classes.resize(count);

    size_t valid_classes = 0;

    for (size_t index = 0U; index < count; ++index)
    {
      auto address = Il2cppApi::get_class_from_image(this, index);
      if (!address)
        continue;

      auto name = Il2cppApi::get_class_name(address);
      if (!name)
        continue;

      if (!strcmp(name, "<Module>"))
        continue;

      m_classes[valid_classes++] = address;
    }

    return m_classes;
  }

  /**
   * Returns the name of the image.
   */
  auto getName() const { return this->name; }
};
