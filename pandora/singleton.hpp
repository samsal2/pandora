#ifndef PANDORA_SINGLETON_HPP_
#define PANDORA_SINGLETON_HPP_

#include <memory>

namespace pandora::utility {

template <typename T> class singleton {
public:
  using type = T;

  static type &the() {
    static auto instance = std::make_unique<type>();
    return *instance;
  }

private:
  singleton() = default;
};

} // namespace pandora::utility

#endif
