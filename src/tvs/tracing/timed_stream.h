/*
 * Copyright (c) 2017 OFFIS Institute for Information Technology
 *                          Oldenburg, Germany
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file   timed_stream.h
 * \author Philipp A. Hartmann <pah@computer.org>
 * \brief  timed value streams for tracing
 * \see    timed_tuple.h
 */

#ifndef TVS_TIMED_STREAM_H_INCLUDED_
#define TVS_TIMED_STREAM_H_INCLUDED_

#include "tvs/tracing/timed_sequence.h"
#include "tvs/tracing/timed_stream_base.h"
#include "tvs/tracing/timed_value.h"

namespace tracing {

template<typename, template<typename> class>
class timed_writer;
template<typename, template<typename> class>
class timed_reader;

template<typename>
struct timed_state_traits;

// timed_stream< int >  -> timed_stream< int, timed_stream_policies< int > >

template<typename T, template<typename> class Traits = timed_state_traits>
class timed_stream : public timed_stream_base,
                     protected Traits<T>::empty_policy,
                     protected Traits<T>::split_policy
{
  friend class timed_writer<T, Traits>;
  friend class timed_reader<T, Traits>;

public:
  typedef timed_stream_base base_type;
  typedef timed_stream<T, Traits> this_type;
  typedef T value_type;
  typedef timed_value<T> tuple_type;

  typedef timed_reader<T, Traits> reader_type;
  typedef timed_writer<T, Traits> writer_type;

  typedef typename Traits<T>::empty_policy empty_policy;
  typedef typename Traits<T>::split_policy split_policy;

  typedef timed_sequence<T, Traits> sequence_type;

  explicit timed_stream(const char* nm = base_type::default_name())
    : base_type(nm)
  {
  }

  virtual duration_type duration() const { return buf_.duration(); }

protected:
  /** \name push interface */
  ///\{

  void push(tuple_type const&);

  void push(value_type const&);

  void push(time_type offset, tuple_type const&);

  ///\}

protected:
  bool do_type_check(timed_reader_base const& r) const
  {
    return (dynamic_cast<reader_type const*>(&r) != NULL);
  }

  bool do_type_check(timed_writer_base const& w) const
  {
    return (dynamic_cast<writer_type const*>(&w) != NULL);
  }

  virtual void do_pre_commit_reader(duration_type const&);
  virtual void do_commit_reader(timed_reader_base& r,
                                duration_type const& until,
                                bool last = false);
  virtual void do_clear() { buf_.clear(); }

private:
  sequence_type buf_;
  sequence_type future_;
};

} // namespace tracing

#endif /* TVS_TIMED_STREAM_H_INCLUDED_ */
/* Taf!
 */
