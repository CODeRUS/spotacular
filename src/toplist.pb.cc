// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: toplist.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "toplist.pb.h"

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

namespace {

const ::google::protobuf::Descriptor* Toplist_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Toplist_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_toplist_2eproto() {
  protobuf_AddDesc_toplist_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "toplist.proto");
  GOOGLE_CHECK(file != NULL);
  Toplist_descriptor_ = file->message_type(0);
  static const int Toplist_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Toplist, items_),
  };
  Toplist_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Toplist_descriptor_,
      Toplist::default_instance_,
      Toplist_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Toplist, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Toplist, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Toplist));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_toplist_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Toplist_descriptor_, &Toplist::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_toplist_2eproto() {
  delete Toplist::default_instance_;
  delete Toplist_reflection_;
}

void protobuf_AddDesc_toplist_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\rtoplist.proto\"\030\n\007Toplist\022\r\n\005items\030\001 \003("
    "\t", 41);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "toplist.proto", &protobuf_RegisterTypes);
  Toplist::default_instance_ = new Toplist();
  Toplist::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_toplist_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_toplist_2eproto {
  StaticDescriptorInitializer_toplist_2eproto() {
    protobuf_AddDesc_toplist_2eproto();
  }
} static_descriptor_initializer_toplist_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int Toplist::kItemsFieldNumber;
#endif  // !_MSC_VER

Toplist::Toplist()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:Toplist)
}

void Toplist::InitAsDefaultInstance() {
}

Toplist::Toplist(const Toplist& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:Toplist)
}

void Toplist::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Toplist::~Toplist() {
  // @@protoc_insertion_point(destructor:Toplist)
  SharedDtor();
}

void Toplist::SharedDtor() {
  if (this != default_instance_) {
  }
}

void Toplist::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Toplist::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Toplist_descriptor_;
}

const Toplist& Toplist::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_toplist_2eproto();
  return *default_instance_;
}

Toplist* Toplist::default_instance_ = NULL;

Toplist* Toplist::New() const {
  return new Toplist;
}

void Toplist::Clear() {
  items_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Toplist::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:Toplist)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated string items = 1;
      case 1: {
        if (tag == 10) {
         parse_items:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->add_items()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->items(this->items_size() - 1).data(),
            this->items(this->items_size() - 1).length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "items");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(10)) goto parse_items;
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
  // @@protoc_insertion_point(parse_success:Toplist)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:Toplist)
  return false;
#undef DO_
}

void Toplist::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:Toplist)
  // repeated string items = 1;
  for (int i = 0; i < this->items_size(); i++) {
  ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
    this->items(i).data(), this->items(i).length(),
    ::google::protobuf::internal::WireFormat::SERIALIZE,
    "items");
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->items(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:Toplist)
}

::google::protobuf::uint8* Toplist::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:Toplist)
  // repeated string items = 1;
  for (int i = 0; i < this->items_size(); i++) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->items(i).data(), this->items(i).length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "items");
    target = ::google::protobuf::internal::WireFormatLite::
      WriteStringToArray(1, this->items(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Toplist)
  return target;
}

int Toplist::ByteSize() const {
  int total_size = 0;

  // repeated string items = 1;
  total_size += 1 * this->items_size();
  for (int i = 0; i < this->items_size(); i++) {
    total_size += ::google::protobuf::internal::WireFormatLite::StringSize(
      this->items(i));
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

void Toplist::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Toplist* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Toplist*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Toplist::MergeFrom(const Toplist& from) {
  GOOGLE_CHECK_NE(&from, this);
  items_.MergeFrom(from.items_);
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Toplist::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Toplist::CopyFrom(const Toplist& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Toplist::IsInitialized() const {

  return true;
}

void Toplist::Swap(Toplist* other) {
  if (other != this) {
    items_.Swap(&other->items_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Toplist::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Toplist_descriptor_;
  metadata.reflection = Toplist_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
