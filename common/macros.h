#pragma once

namespace frad {

#define DISALLOW_COPY_ASSIGN(Class) \
  Class(const Class&) = delete;     \
  Class& operator=(const Class&) = delete;

#define DISALLOW_COPY_MOVE_ASSIGN(Class)   \
  Class(const Class&) = delete;            \
  Class(Class&&) = delete;                 \
  Class& operator=(const Class&) = delete; \
  Class& operator=(Class&&) = delete;

}  // namespace frad
