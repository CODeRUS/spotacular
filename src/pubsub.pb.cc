// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: pubsub.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "pubsub.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace spotify {
namespace hermes {
namespace pubsub {
namespace proto {

namespace {

const ::google::protobuf::Descriptor* Subscription_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Subscription_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_pubsub_2eproto() {
  protobuf_AddDesc_pubsub_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "pubsub.proto");
  GOOGLE_CHECK(file != NULL);
  Subscription_descriptor_ = file->message_type(0);
  static const int Subscription_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Subscription, uri_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Subscription, expiry_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Subscription, status_code_),
  };
  Subscription_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Subscription_descriptor_,
      Subscription::default_instance_,
      Subscription_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Subscription, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Subscription, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Subscription));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_pubsub_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Subscription_descriptor_, &Subscription::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_pubsub_2eproto() {
  delete Subscription::default_instance_;
  delete Subscription_reflection_;
}

void protobuf_AddDesc_pubsub_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\014pubsub.proto\022\033spotify.hermes.pubsub.pr"
    "oto\"@\n\014Subscription\022\013\n\003uri\030\001 \001(\t\022\016\n\006expi"
    "ry\030\002 \001(\005\022\023\n\013status_code\030\003 \001(\005", 109);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "pubsub.proto", &protobuf_RegisterTypes);
  Subscription::default_instance_ = new Subscription();
  Subscription::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_pubsub_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_pubsub_2eproto {
  StaticDescriptorInitializer_pubsub_2eproto() {
    protobuf_AddDesc_pubsub_2eproto();
  }
} static_descriptor_initializer_pubsub_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int Subscription::kUriFieldNumber;
const int Subscription::kExpiryFieldNumber;
const int Subscription::kStatusCodeFieldNumber;
#endif  // !_MSC_VER

Subscription::Subscription()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:spotify.hermes.pubsub.proto.Subscription)
}

void Subscription::InitAsDefaultInstance() {
}

Subscription::Subscription(const Subscription& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:spotify.hermes.pubsub.proto.Subscription)
}

void Subscription::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  uri_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  expiry_ = 0;
  status_code_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Subscription::~Subscription() {
  // @@protoc_insertion_point(destructor:spotify.hermes.pubsub.proto.Subscription)
  SharedDtor();
}

void Subscription::SharedDtor() {
  if (uri_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete uri_;
  }
  if (this != default_instance_) {
  }
}

void Subscription::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Subscription::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Subscription_descriptor_;
}

const Subscription& Subscription::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_pubsub_2eproto();
  return *default_instance_;
}

Subscription* Subscription::default_instance_ = NULL;

Subscription* Subscription::New() const {
  return new Subscription;
}

void Subscription::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<Subscription*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  if (_has_bits_[0 / 32] & 7) {
    ZR_(expiry_, status_code_);
    if (has_uri()) {
      if (uri_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        uri_->clear();
      }
    }
  }

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Subscription::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:spotify.hermes.pubsub.proto.Subscription)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string uri = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_uri()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->uri().data(), this->uri().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "uri");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_expiry;
        break;
      }

      // optional int32 expiry = 2;
      case 2: {
        if (tag == 16) {
         parse_expiry:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &expiry_)));
          set_has_expiry();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(24)) goto parse_status_code;
        break;
      }

      // optional int32 status_code = 3;
      case 3: {
        if (tag == 24) {
         parse_status_code:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &status_code_)));
          set_has_status_code();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:spotify.hermes.pubsub.proto.Subscription)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:spotify.hermes.pubsub.proto.Subscription)
  return false;
#undef DO_
}

void Subscription::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:spotify.hermes.pubsub.proto.Subscription)
  // optional string uri = 1;
  if (has_uri()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->uri().data(), this->uri().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "uri");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->uri(), output);
  }

  // optional int32 expiry = 2;
  if (has_expiry()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->expiry(), output);
  }

  // optional int32 status_code = 3;
  if (has_status_code()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->status_code(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:spotify.hermes.pubsub.proto.Subscription)
}

::google::protobuf::uint8* Subscription::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:spotify.hermes.pubsub.proto.Subscription)
  // optional string uri = 1;
  if (has_uri()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->uri().data(), this->uri().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "uri");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->uri(), target);
  }

  // optional int32 expiry = 2;
  if (has_expiry()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->expiry(), target);
  }

  // optional int32 status_code = 3;
  if (has_status_code()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->status_code(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:spotify.hermes.pubsub.proto.Subscription)
  return target;
}

int Subscription::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string uri = 1;
    if (has_uri()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->uri());
    }

    // optional int32 expiry = 2;
    if (has_expiry()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->expiry());
    }

    // optional int32 status_code = 3;
    if (has_status_code()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->status_code());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Subscription::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Subscription* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Subscription*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Subscription::MergeFrom(const Subscription& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_uri()) {
      set_uri(from.uri());
    }
    if (from.has_expiry()) {
      set_expiry(from.expiry());
    }
    if (from.has_status_code()) {
      set_status_code(from.status_code());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Subscription::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Subscription::CopyFrom(const Subscription& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Subscription::IsInitialized() const {

  return true;
}

void Subscription::Swap(Subscription* other) {
  if (other != this) {
    std::swap(uri_, other->uri_);
    std::swap(expiry_, other->expiry_);
    std::swap(status_code_, other->status_code_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Subscription::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Subscription_descriptor_;
  metadata.reflection = Subscription_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto
}  // namespace pubsub
}  // namespace hermes
}  // namespace spotify

// @@protoc_insertion_point(global_scope)