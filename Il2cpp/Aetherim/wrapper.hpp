#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

#include "./image.hpp"

struct Wrapper
{
public:
  using images_t = std::vector<Il2cppImage *>;

private:
  images_t m_images{};

public:
  /**
   * Dumps all images.
   */
  Wrapper()
  {
    size_t count = 0U;

    const auto domain = Il2cppApi::get_domain();
    Il2cppApi::thread_attach(domain);

    const auto assemblies = Il2cppApi::get_assemblies(domain, &count);

    for (size_t index = 0U; index < count; ++index)
    {
      auto image = Il2cppApi::get_image(assemblies[index]);
      if (image)
        m_images.emplace_back(static_cast<Il2cppImage *>(image));
    }
  }

  /**
   * Returns a pointer to the desired image if found.
   */
  auto getImage(const char *name) const -> Il2cppImage *
  {
    for (const auto image : m_images)
    {
      if (!strcmp(name, image->getName()))
        return image;
    }

    return nullptr;
  }

  /**
   * Returns all images.
   */
  auto getImages() const -> images_t { return m_images; }

  /**
   * Checks if there's an active debugger attached to the thread.
   */
  auto isDebuggerAttached() const -> bool { return Il2cppApi::is_debugger_attached(); }
};
