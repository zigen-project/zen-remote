#pragma once

#include "core/common.h"
#include "zen-remote/server/gl-program.h"

namespace zen::remote::server {

class Channel;

class GlProgram final : public IGlProgram {
 public:
  DISABLE_MOVE_AND_COPY(GlProgram);
  GlProgram() = delete;
  GlProgram(std::shared_ptr<Channel> channel);
  ~GlProgram();

  void Init();

  void GlAttachShader(uint64_t shader_id) override;

  void GlLinkProgram() override;

  uint64_t id() override;

 private:
  uint64_t id_;
  std::weak_ptr<Channel> channel_;
};

}  // namespace zen::remote::server
