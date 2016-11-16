// Copyright (c) 2015 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ATOM_BROWSER_NET_ATOM_HOST_RESOLVER_H_
#define ATOM_BROWSER_NET_ATOM_HOST_RESOLVER_H_

#include <memory>
#include <string>

#include "net/dns/host_resolver.h"

namespace atom {

class AtomHostResolver : public net::HostResolver {
 public:
  explicit AtomHostResolver(std::unique_ptr<net::HostResolver> impl);
  virtual ~AtomHostResolver();

  using ResolveProc =
      base::Callback<void(const std::string& hostname,
                          const base::Callback<void(const std::string&)>&)>;

  void SetResolveProc(const ResolveProc& proc);

 protected:
  // net::CertVerifier:
  int Resolve(const RequestInfo& info,
              net::RequestPriority priority,
              net::AddressList* addresses,
              const net::CompletionCallback& callback,
              RequestHandle* out_req,
              const net::BoundNetLog& net_log) override;
  int ResolveFromCache(const RequestInfo& info,
                       net::AddressList* addresses,
                       const net::BoundNetLog& net_log) override;
  void CancelRequest(RequestHandle req) override;
  void SetDnsClientEnabled(bool enabled) override;
  net::HostCache* GetHostCache() override;
  std::unique_ptr<base::Value> GetDnsConfigAsValue() const override;

 private:
  std::unique_ptr<net::HostResolver> impl_;
  ResolveProc resolve_proc_;

  DISALLOW_COPY_AND_ASSIGN(AtomHostResolver);
};

}   // namespace atom

#endif  // ATOM_BROWSER_NET_ATOM_HOST_RESOLVER_H_
