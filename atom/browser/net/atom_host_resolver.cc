// Copyright (c) 2015 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "atom/browser/net/atom_host_resolver.h"

#include "atom/browser/browser.h"
#include "content/public/browser/browser_thread.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_errors.h"

using content::BrowserThread;

namespace atom {

namespace {

void CallImpl(std::unique_ptr<net::HostResolver> impl,
              const net::HostResolver::RequestInfo& info,
              net::RequestPriority priority,
              net::AddressList* addresses,
              const net::CompletionCallback& callback,
              net::HostResolver::RequestHandle* out_req,
              const net::BoundNetLog& net_log) {
  int result =
      impl->Resolve(info, priority, addresses, callback, out_req, net_log);
  if (result != net::ERR_IO_PENDING) {
    // TODO call directly
    //callback(result);
  }
}

void OnProcResult(std::unique_ptr<net::HostResolver> impl,
                  net::HostResolver::RequestInfo* info,
                  net::RequestPriority priority,
                  net::AddressList* addresses,
                  const net::CompletionCallback& callback,
                  net::HostResolver::RequestHandle* out_req,
                  const net::BoundNetLog& net_log,
                  const std::string& result) {
  // TODO null?
  if (result == "-NOTFOUND") {
    BrowserThread::PostTask(
        BrowserThread::IO, FROM_HERE,
        base::Bind(callback, net::ERR_NAME_NOT_RESOLVED));
  }
  else {
    net::HostPortPair host_port(info->host_port_pair());
    host_port.set_host(result);
    info->set_host_port_pair(host_port);

    BrowserThread::PostTask(
        BrowserThread::IO, FROM_HERE,
        base::Bind(
            CallImpl, impl, info, priority, addresses, callback, out_req,
            net_log));
  }
}

}  // namespace

AtomHostResolver::AtomHostResolver(std::unique_ptr<HostResolver> impl)
    : impl_(std::move(impl)) {}

AtomHostResolver::~AtomHostResolver() {}

void AtomHostResolver::SetResolveProc(const ResolveProc& proc) {
  resolve_proc_ = proc;
}

int AtomHostResolver::Resolve(
    const net::HostResolver::RequestInfo& info,
    net::RequestPriority priority,
    net::AddressList* addresses,
    const net::CompletionCallback& callback,
    net::HostResolver::RequestHandle* out_req,
    const net::BoundNetLog& net_log) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  if (resolve_proc_.is_null()) {
    return impl_->Resolve(
        info, priority, addresses, callback, out_req, net_log);
  }

  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(resolve_proc_, info.host_port_pair().host(),
                 base::Bind(
                     OnProcResult, impl_, info, priority, addresses, callback,
                     out_req, net_log)));
  return net::ERR_IO_PENDING;
}

int AtomHostResolver::ResolveFromCache(
    const net::HostResolver::RequestInfo& info,
    net::AddressList* addresses,
    const net::BoundNetLog& net_log) {
  return impl_->ResolveFromCache(info, addresses, net_log);
}

void AtomHostResolver::CancelRequest(RequestHandle req) {
  impl_->CancelRequest(req);
}

void AtomHostResolver::SetDnsClientEnabled(bool enabled) {
  impl_->SetDnsClientEnabled(enabled);
}

net::HostCache* AtomHostResolver::GetHostCache() {
  return impl_->GetHostCache();
}

std::unique_ptr<base::Value> AtomHostResolver::GetDnsConfigAsValue() const {
  return impl_->GetDnsConfigAsValue();
}

}  // namespace atom
