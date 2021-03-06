// Copyright (c) 2006, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// minidump_file_writer-inl.h: Minidump file writer implementation.
//
// See minidump_file_writer.h for documentation.

#ifndef CLIENT_MINIDUMP_FILE_WRITER_INL_H__
#define CLIENT_MINIDUMP_FILE_WRITER_INL_H__

#include <assert.h>

#include "client/minidump_file_writer.h"
#include "google_breakpad/common/minidump_size.h"

//#define DEBUG_LOG(...)
namespace google_breakpad {

template<typename MDType>
inline bool TypedMDRVA<MDType>::Allocate() {
  allocation_state_ = SINGLE_OBJECT;
  DEBUG_LOG("Allocate MDType\n");
  DEBUG_LOG("Allocate size:%lu\n", minidump_size<MDType>::size());
  return UntypedMDRVA::Allocate(minidump_size<MDType>::size());
}

template<typename MDType>
inline bool TypedMDRVA<MDType>::Allocate(size_t additional) {
  allocation_state_ = SINGLE_OBJECT;
  DEBUG_LOG("Allocate additional size:%lu\n", minidump_size<MDType>::size()+additional);
  return UntypedMDRVA::Allocate(minidump_size<MDType>::size() + additional);
}

template<typename MDType>
inline bool TypedMDRVA<MDType>::AllocateArray(size_t count) {
  assert(count);
  allocation_state_ = ARRAY;
  DEBUG_LOG("AllocateArray size:%lu\n", minidump_size<MDType>::size() * count);
  return UntypedMDRVA::Allocate(minidump_size<MDType>::size() * count);
}

template<typename MDType>
inline bool TypedMDRVA<MDType>::AllocateObjectAndArray(size_t count,
                                                       size_t length) {
  assert(count && length);
  allocation_state_ = SINGLE_OBJECT_WITH_ARRAY;
  DEBUG_LOG("AllocateObjectAndArray size:%lu, count:%lu, length:%lu\n", minidump_size<MDType>::size() + count * length, count, length);
  return UntypedMDRVA::Allocate(minidump_size<MDType>::size() + count * length);
}

template<typename MDType>
inline bool TypedMDRVA<MDType>::CopyIndex(unsigned int index, MDType *item) {
  assert(allocation_state_ == ARRAY);
  DEBUG_LOG("[copyindex]index:%d, position:%d,size:%lu\n", index,position_, minidump_size<MDType>::size());
  return writer_->Copy(
      static_cast<MDRVA>(position_ + index * minidump_size<MDType>::size()), 
      item, minidump_size<MDType>::size());
}

template<typename MDType>
inline bool TypedMDRVA<MDType>::CopyIndexAfterObject(unsigned int index,
                                                     const void *src, 
                                                     size_t length) {
  assert(allocation_state_ == SINGLE_OBJECT_WITH_ARRAY);
  DEBUG_LOG("CopyIndexAfterObject:index:%d,length:%lu,position:%d,typesize:%lu\n", 
  	index,length,position_,minidump_size<MDType>::size());
  return writer_->Copy(
      static_cast<MDRVA>(position_ + minidump_size<MDType>::size() 
                         + index * length),
      src, length);
}

template<typename MDType>
inline bool TypedMDRVA<MDType>::Flush() {
  return writer_->Copy(position_, &data_, minidump_size<MDType>::size());
}

}  // namespace google_breakpad

#endif  // CLIENT_MINIDUMP_FILE_WRITER_INL_H__
