#pragma once

#include "client/resource.h"
#include "core/common.h"

namespace zen::remote::client {

class AtomicCommandQueue;

class GlBuffer final : public IResource {
  struct RenderingState {
    GLuint buffer_id = 0;

    uint32_t target;
  };

 public:
  DISABLE_MOVE_AND_COPY(GlBuffer);
  GlBuffer() = delete;
  GlBuffer(uint64_t id, AtomicCommandQueue *update_rendering_queue);
  ~GlBuffer();

  /** Used in the update thread */
  void Commit();

  /** Used in the update thread */
  void GlBufferData(
      const void *data, uint32_t target, size_t size, uint32_t usage);

  uint64_t id() override;

  /** Used in the rendering thread */
  inline GLuint buffer_id();
  inline uint32_t target();

 private:
  const uint64_t id_;
  AtomicCommandQueue *update_rendering_queue_;

  struct {
    void *data = NULL;
    size_t size = 0;
    size_t alloc = 0;

    uint32_t target;
    uint32_t usage;

    bool data_damaged = false;
  } pending_;

  std::shared_ptr<RenderingState> rendering_;
};

inline GLuint
GlBuffer::buffer_id()
{
  return rendering_->buffer_id;
}

inline uint32_t
GlBuffer::target()
{
  return rendering_->target;
}

}  // namespace zen::remote::client
