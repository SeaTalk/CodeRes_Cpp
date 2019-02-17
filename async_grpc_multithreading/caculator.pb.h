// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: caculator.proto

#ifndef PROTOBUF_INCLUDED_caculator_2eproto
#define PROTOBUF_INCLUDED_caculator_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_caculator_2eproto 

namespace protobuf_caculator_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_caculator_2eproto
namespace caculator {
class Request;
class RequestDefaultTypeInternal;
extern RequestDefaultTypeInternal _Request_default_instance_;
class Response;
class ResponseDefaultTypeInternal;
extern ResponseDefaultTypeInternal _Response_default_instance_;
}  // namespace caculator
namespace google {
namespace protobuf {
template<> ::caculator::Request* Arena::CreateMaybeMessage<::caculator::Request>(Arena*);
template<> ::caculator::Response* Arena::CreateMaybeMessage<::caculator::Response>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace caculator {

// ===================================================================

class Request : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:caculator.Request) */ {
 public:
  Request();
  virtual ~Request();

  Request(const Request& from);

  inline Request& operator=(const Request& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Request(Request&& from) noexcept
    : Request() {
    *this = ::std::move(from);
  }

  inline Request& operator=(Request&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Request& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Request* internal_default_instance() {
    return reinterpret_cast<const Request*>(
               &_Request_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Request* other);
  friend void swap(Request& a, Request& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Request* New() const final {
    return CreateMaybeMessage<Request>(NULL);
  }

  Request* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Request>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Request& from);
  void MergeFrom(const Request& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Request* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string opname = 1;
  void clear_opname();
  static const int kOpnameFieldNumber = 1;
  const ::std::string& opname() const;
  void set_opname(const ::std::string& value);
  #if LANG_CXX11
  void set_opname(::std::string&& value);
  #endif
  void set_opname(const char* value);
  void set_opname(const char* value, size_t size);
  ::std::string* mutable_opname();
  ::std::string* release_opname();
  void set_allocated_opname(::std::string* opname);

  // int32 op1 = 2;
  void clear_op1();
  static const int kOp1FieldNumber = 2;
  ::google::protobuf::int32 op1() const;
  void set_op1(::google::protobuf::int32 value);

  // int32 op2 = 3;
  void clear_op2();
  static const int kOp2FieldNumber = 3;
  ::google::protobuf::int32 op2() const;
  void set_op2(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:caculator.Request)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr opname_;
  ::google::protobuf::int32 op1_;
  ::google::protobuf::int32 op2_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_caculator_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Response : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:caculator.Response) */ {
 public:
  Response();
  virtual ~Response();

  Response(const Response& from);

  inline Response& operator=(const Response& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Response(Response&& from) noexcept
    : Response() {
    *this = ::std::move(from);
  }

  inline Response& operator=(Response&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Response& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Response* internal_default_instance() {
    return reinterpret_cast<const Response*>(
               &_Response_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(Response* other);
  friend void swap(Response& a, Response& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Response* New() const final {
    return CreateMaybeMessage<Response>(NULL);
  }

  Response* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Response>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Response& from);
  void MergeFrom(const Response& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Response* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // int32 result = 1;
  void clear_result();
  static const int kResultFieldNumber = 1;
  ::google::protobuf::int32 result() const;
  void set_result(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:caculator.Response)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::int32 result_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_caculator_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Request

// string opname = 1;
inline void Request::clear_opname() {
  opname_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Request::opname() const {
  // @@protoc_insertion_point(field_get:caculator.Request.opname)
  return opname_.GetNoArena();
}
inline void Request::set_opname(const ::std::string& value) {
  
  opname_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:caculator.Request.opname)
}
#if LANG_CXX11
inline void Request::set_opname(::std::string&& value) {
  
  opname_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:caculator.Request.opname)
}
#endif
inline void Request::set_opname(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  opname_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:caculator.Request.opname)
}
inline void Request::set_opname(const char* value, size_t size) {
  
  opname_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:caculator.Request.opname)
}
inline ::std::string* Request::mutable_opname() {
  
  // @@protoc_insertion_point(field_mutable:caculator.Request.opname)
  return opname_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Request::release_opname() {
  // @@protoc_insertion_point(field_release:caculator.Request.opname)
  
  return opname_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Request::set_allocated_opname(::std::string* opname) {
  if (opname != NULL) {
    
  } else {
    
  }
  opname_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), opname);
  // @@protoc_insertion_point(field_set_allocated:caculator.Request.opname)
}

// int32 op1 = 2;
inline void Request::clear_op1() {
  op1_ = 0;
}
inline ::google::protobuf::int32 Request::op1() const {
  // @@protoc_insertion_point(field_get:caculator.Request.op1)
  return op1_;
}
inline void Request::set_op1(::google::protobuf::int32 value) {
  
  op1_ = value;
  // @@protoc_insertion_point(field_set:caculator.Request.op1)
}

// int32 op2 = 3;
inline void Request::clear_op2() {
  op2_ = 0;
}
inline ::google::protobuf::int32 Request::op2() const {
  // @@protoc_insertion_point(field_get:caculator.Request.op2)
  return op2_;
}
inline void Request::set_op2(::google::protobuf::int32 value) {
  
  op2_ = value;
  // @@protoc_insertion_point(field_set:caculator.Request.op2)
}

// -------------------------------------------------------------------

// Response

// int32 result = 1;
inline void Response::clear_result() {
  result_ = 0;
}
inline ::google::protobuf::int32 Response::result() const {
  // @@protoc_insertion_point(field_get:caculator.Response.result)
  return result_;
}
inline void Response::set_result(::google::protobuf::int32 value) {
  
  result_ = value;
  // @@protoc_insertion_point(field_set:caculator.Response.result)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace caculator

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_caculator_2eproto
