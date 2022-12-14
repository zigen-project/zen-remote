#include "server/gl-sampler.h"

#include <GLES3/gl32.h>

#include "core/logger.h"
#include "gl-sampler.grpc.pb.h"
#include "server/async-grpc-caller.h"
#include "server/async-grpc-queue.h"
#include "server/channel.h"
#include "server/serial-request-context.h"

namespace zen::remote::server {

namespace {

size_t
CalcParameterSize(uint32_t pname)
{
  switch (pname) {
    case GL_TEXTURE_MIN_FILTER:
    case GL_TEXTURE_MAG_FILTER:
    case GL_TEXTURE_WRAP_S:
    case GL_TEXTURE_WRAP_T:
    case GL_TEXTURE_WRAP_R:
    case GL_TEXTURE_MIN_LOD:
    case GL_TEXTURE_MAX_LOD:
    case GL_TEXTURE_COMPARE_MODE:
    case GL_TEXTURE_COMPARE_FUNC:
      return 4;

    case GL_TEXTURE_BORDER_COLOR:
      return 16;

    default:
      return 0;
  }
}

}  // namespace

GlSampler::GlSampler(std::shared_ptr<Channel> channel)
    : id_(channel->NewSerial(Channel::kResource)), channel_(std::move(channel))
{
}

void
GlSampler::Init()
{
  auto job = CreateJob([id = id_, channel_weak = channel_](bool cancel) {
    auto channel = channel_weak.lock();
    if (cancel || !channel) {
      return;
    }

    auto context =
        std::unique_ptr<grpc::ClientContext>(new SerialRequestContext(channel));

    auto stub = GlSamplerService::NewStub(channel->grpc_channel());

    auto caller = new AsyncGrpcCaller<&GlSamplerService::Stub::PrepareAsyncNew>(
        std::move(stub), std::move(context),
        [channel_weak](EmptyResponse* /*response*/, grpc::Status* status) {
          if (!status->ok() && status->error_code() != grpc::CANCELLED) {
            LOG_WARN("Failed to call remote GlSampler::New");
            if (auto channel = channel_weak.lock())
              channel->NotifyDisconnection();
          }
        });

    caller->request()->set_id(id);

    channel->PushGrpcCaller(std::unique_ptr<AsyncGrpcCallerBase>(caller));
  });

  if (auto channel = channel_.lock()) {
    channel->PushJob(std::move(job));
  }
}

void
GlSampler::GlSamplerParameterf(uint32_t pname, float param)
{
  auto job = CreateJob([id = id_, channel_weak = channel_, pname, param](
                           bool cancel) {
    auto channel = channel_weak.lock();
    if (cancel || !channel) {
      return;
    }

    auto context =
        std::unique_ptr<grpc::ClientContext>(new SerialRequestContext(channel));

    auto stub = GlSamplerService::NewStub(channel->grpc_channel());

    auto caller = new AsyncGrpcCaller<
        &GlSamplerService::Stub::PrepareAsyncGlSamplerParameterf>(
        std::move(stub), std::move(context),
        [channel_weak](EmptyResponse* /*response*/, grpc::Status* status) {
          if (!status->ok() && status->error_code() != grpc::CANCELLED) {
            LOG_WARN("Failed to call remote GlSampler::GlSamplerParameterf");
            if (auto channel = channel_weak.lock())
              channel->NotifyDisconnection();
          }
        });

    caller->request()->set_id(id);
    caller->request()->set_pname(pname);
    caller->request()->set_param(param);

    channel->PushGrpcCaller(std::unique_ptr<AsyncGrpcCallerBase>(caller));
  });

  if (auto channel = channel_.lock()) {
    channel->PushJob(std::move(job));
  }
};

void
GlSampler::GlSamplerParameteri(uint32_t pname, int32_t param)
{
  auto job = CreateJob([id = id_, channel_weak = channel_, pname, param](
                           bool cancel) {
    auto channel = channel_weak.lock();
    if (cancel || !channel) {
      return;
    }

    auto context =
        std::unique_ptr<grpc::ClientContext>(new SerialRequestContext(channel));

    auto stub = GlSamplerService::NewStub(channel->grpc_channel());

    auto caller = new AsyncGrpcCaller<
        &GlSamplerService::Stub::PrepareAsyncGlSamplerParameteri>(
        std::move(stub), std::move(context),
        [channel_weak](EmptyResponse* /*response*/, grpc::Status* status) {
          if (!status->ok() && status->error_code() != grpc::CANCELLED) {
            LOG_WARN("Failed to call remote GlSampler::GlSamplerParameteri");
            if (auto channel = channel_weak.lock())
              channel->NotifyDisconnection();
          }
        });

    caller->request()->set_id(id);
    caller->request()->set_pname(pname);
    caller->request()->set_param(param);

    channel->PushGrpcCaller(std::unique_ptr<AsyncGrpcCallerBase>(caller));
  });

  if (auto channel = channel_.lock()) {
    channel->PushJob(std::move(job));
  }
};

void
GlSampler::GlSamplerParameterfv(uint32_t pname, const float* params)
{
  size_t params_size = CalcParameterSize(pname);
  std::string params_copy(params_size, ' ');
  std::memcpy(params_copy.data(), params, params_size);

  auto job = CreateJob([id = id_, channel_weak = channel_, pname,
                           params = std::move(params_copy)](bool cancel) {
    auto channel = channel_weak.lock();
    if (cancel || !channel) {
      return;
    }

    auto context =
        std::unique_ptr<grpc::ClientContext>(new SerialRequestContext(channel));

    auto stub = GlSamplerService::NewStub(channel->grpc_channel());

    auto caller = new AsyncGrpcCaller<
        &GlSamplerService::Stub::PrepareAsyncGlSamplerParameterfv>(
        std::move(stub), std::move(context),
        [channel_weak](EmptyResponse* /*response*/, grpc::Status* status) {
          if (!status->ok() && status->error_code() != grpc::CANCELLED) {
            LOG_WARN("Failed to call remote GlSampler::GlSamplerParameterfv");
            if (auto channel = channel_weak.lock())
              channel->NotifyDisconnection();
          }
        });

    caller->request()->set_id(id);
    caller->request()->set_pname(pname);
    caller->request()->set_params(std::move(params));

    channel->PushGrpcCaller(std::unique_ptr<AsyncGrpcCallerBase>(caller));
  });

  if (auto channel = channel_.lock()) {
    channel->PushJob(std::move(job));
  }
};

void
GlSampler::GlSamplerParameteriv(uint32_t pname, const int32_t* params)
{
  size_t params_size = CalcParameterSize(pname);
  std::string params_copy(params_size, ' ');
  std::memcpy(params_copy.data(), params, params_size);

  auto job = CreateJob([id = id_, channel_weak = channel_, pname,
                           params = std::move(params_copy)](bool cancel) {
    auto channel = channel_weak.lock();
    if (cancel || !channel) {
      return;
    }

    auto context =
        std::unique_ptr<grpc::ClientContext>(new SerialRequestContext(channel));

    auto stub = GlSamplerService::NewStub(channel->grpc_channel());

    auto caller = new AsyncGrpcCaller<
        &GlSamplerService::Stub::PrepareAsyncGlSamplerParameteriv>(
        std::move(stub), std::move(context),
        [channel_weak](EmptyResponse* /*response*/, grpc::Status* status) {
          if (!status->ok() && status->error_code() != grpc::CANCELLED) {
            LOG_WARN("Failed to call remote GlSampler::GlSamplerParameteriv");
            if (auto channel = channel_weak.lock())
              channel->NotifyDisconnection();
          }
        });

    caller->request()->set_id(id);
    caller->request()->set_pname(pname);
    caller->request()->set_params(std::move(params));

    channel->PushGrpcCaller(std::unique_ptr<AsyncGrpcCallerBase>(caller));
  });

  if (auto channel = channel_.lock()) {
    channel->PushJob(std::move(job));
  }
};

void
GlSampler::GlSamplerParameterIiv(uint32_t pname, const int32_t* params)
{
  size_t params_size = CalcParameterSize(pname);
  std::string params_copy(params_size, ' ');
  std::memcpy(params_copy.data(), params, params_size);

  auto job = CreateJob([id = id_, channel_weak = channel_, pname,
                           params = std::move(params_copy)](bool cancel) {
    auto channel = channel_weak.lock();
    if (cancel || !channel) {
      return;
    }

    auto context =
        std::unique_ptr<grpc::ClientContext>(new SerialRequestContext(channel));

    auto stub = GlSamplerService::NewStub(channel->grpc_channel());

    auto caller = new AsyncGrpcCaller<
        &GlSamplerService::Stub::PrepareAsyncGlSamplerParameterIiv>(
        std::move(stub), std::move(context),
        [channel_weak](EmptyResponse* /*response*/, grpc::Status* status) {
          if (!status->ok() && status->error_code() != grpc::CANCELLED) {
            LOG_WARN("Failed to call remote GlSampler::GlSamplerParameterIiv");
            if (auto channel = channel_weak.lock())
              channel->NotifyDisconnection();
          }
        });

    caller->request()->set_id(id);
    caller->request()->set_pname(pname);
    caller->request()->set_params(std::move(params));

    channel->PushGrpcCaller(std::unique_ptr<AsyncGrpcCallerBase>(caller));
  });

  if (auto channel = channel_.lock()) {
    channel->PushJob(std::move(job));
  }
};

void
GlSampler::GlSamplerParameterIuiv(uint32_t pname, const uint32_t* params)
{
  size_t params_size = CalcParameterSize(pname);
  std::string params_copy(params_size, ' ');
  std::memcpy(params_copy.data(), params, params_size);

  auto job = CreateJob([id = id_, channel_weak = channel_, pname,
                           params = std::move(params_copy)](bool cancel) {
    auto channel = channel_weak.lock();
    if (cancel || !channel) {
      return;
    }

    auto context =
        std::unique_ptr<grpc::ClientContext>(new SerialRequestContext(channel));

    auto stub = GlSamplerService::NewStub(channel->grpc_channel());

    auto caller = new AsyncGrpcCaller<
        &GlSamplerService::Stub::PrepareAsyncGlSamplerParameterIuiv>(
        std::move(stub), std::move(context),
        [channel_weak](EmptyResponse* /*response*/, grpc::Status* status) {
          if (!status->ok() && status->error_code() != grpc::CANCELLED) {
            LOG_WARN("Failed to call remote GlSampler::GlSamplerParameterIuiv");
            if (auto channel = channel_weak.lock())
              channel->NotifyDisconnection();
          }
        });

    caller->request()->set_id(id);
    caller->request()->set_pname(pname);
    caller->request()->set_params(std::move(params));

    channel->PushGrpcCaller(std::unique_ptr<AsyncGrpcCallerBase>(caller));
  });

  if (auto channel = channel_.lock()) {
    channel->PushJob(std::move(job));
  }
};

GlSampler::~GlSampler()
{
  auto job = CreateJob([id = id_, channel_weak = channel_](bool cancel) {
    auto channel = channel_weak.lock();
    if (cancel || !channel) {
      return;
    }

    auto context =
        std::unique_ptr<grpc::ClientContext>(new SerialRequestContext(channel));

    auto stub = GlSamplerService::NewStub(channel->grpc_channel());

    auto caller =
        new AsyncGrpcCaller<&GlSamplerService::Stub::PrepareAsyncDelete>(
            std::move(stub), std::move(context),
            [channel_weak](EmptyResponse* /*response*/, grpc::Status* status) {
              if (!status->ok() && status->error_code() != grpc::CANCELLED) {
                LOG_WARN("Failed to call remote GlSampler::Delete");
                if (auto channel = channel_weak.lock())
                  channel->NotifyDisconnection();
              }
            });

    caller->request()->set_id(id);

    channel->PushGrpcCaller(std::unique_ptr<AsyncGrpcCallerBase>(caller));
  });

  if (auto channel = channel_.lock()) {
    channel->PushJob(std::move(job));
  }
}

uint64_t
GlSampler::id()
{
  return id_;
}

std::unique_ptr<IGlSampler>
CreateGlSampler(std::shared_ptr<IChannel> channel)
{
  auto gl_sampler =
      std::make_unique<GlSampler>(std::dynamic_pointer_cast<Channel>(channel));
  gl_sampler->Init();

  return gl_sampler;
}

}  // namespace zen::remote::server
