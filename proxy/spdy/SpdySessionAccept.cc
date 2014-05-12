/** @file

  SpdyNetAccept

  @section license License

  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 */

#include "SpdySessionAccept.h"
#include "Error.h"

#if TS_HAS_SPDY
#include "SpdySM.h"
#endif

SpdySessionAccept::SpdySessionAccept(Continuation *ep)
    : SessionAccept(new_ProxyMutex()), endpoint(ep)
{
#if TS_HAS_SPDY
  spdy_config_load();
#endif
  SET_HANDLER(&SpdySessionAccept::mainEvent);
}

int
SpdySessionAccept::mainEvent(int event, void * edata)
{
  if (event == NET_EVENT_ACCEPT) {
    NetVConnection * netvc =static_cast<NetVConnection *>(edata);

#if TS_HAS_SPDY
    spdy_sm_create(netvc, NULL, NULL);
#else
    Error("accepted a SPDY session, but SPDY support is not available");
    netvc->do_io_close();
#endif

    return EVENT_CONT;
  }

  MachineFatal("SPDY accept received fatal error: errno = %d", -((int)(intptr_t)edata));
  return EVENT_CONT;
}

void
SpdySessionAccept::accept(NetVConnection * netvc, MIOBuffer * iobuf, IOBufferReader * reader)
{
#if TS_HAS_SPDY
  spdy_sm_create(netvc, iobuf, reader);
#endif
}
