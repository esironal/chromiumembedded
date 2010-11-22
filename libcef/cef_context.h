// Copyright (c) 2010 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#ifndef _CEF_CONTEXT_H
#define _CEF_CONTEXT_H

#include "../include/cef.h"
#include "browser_request_context.h"
#include "cef_process.h"
#include "cef_thread.h"
#include "dom_storage_context.h"

#include "base/at_exit.h"
#include "base/file_path.h"
#include "base/message_loop.h"
#include "base/ref_counted.h"
#include <map>

class BrowserRequestContext;
class CefBrowserImpl;

class CefContext : public CefThreadSafeBase<CefBase>
{
public:
  typedef std::list<CefRefPtr<CefBrowserImpl> > BrowserList;
  
  CefContext();
  ~CefContext();

  // These methods will be called on the main application thread.
  bool Initialize(const CefSettings& settings,
                  const CefBrowserSettings& browser_defaults);
  void Shutdown();

  scoped_refptr<CefProcess> process() { return process_; }

  bool AddBrowser(CefRefPtr<CefBrowserImpl> browser);
  bool RemoveBrowser(CefRefPtr<CefBrowserImpl> browser);
  CefRefPtr<CefBrowserImpl> GetBrowserByID(int id);
  BrowserList* GetBrowserList() { return &browserlist_; }

  // Retrieve the path at which cache data will be stored on disk.  If empty,
  // cache data will be stored in-memory.
  const FilePath& cache_path() const { return cache_path_; }

  const CefSettings& settings() const { return settings_; }
  const CefBrowserSettings& browser_defaults() const
    { return browser_defaults_; }

  // The BrowserRequestContext object is managed by CefProcessIOThread.
  void set_request_context(BrowserRequestContext* request_context)
    { request_context_ = request_context; }
  scoped_refptr<BrowserRequestContext> request_context()
    { return request_context_; }

  // The DOMStorageContext object is managed by CefProcessUIThread.
  void set_storage_context(DOMStorageContext* storage_context)
    { storage_context_.reset(storage_context); }
  DOMStorageContext* storage_context() { return storage_context_.get(); }

private:
  // Manages the various process threads.
  scoped_refptr<CefProcess> process_;

  // Initialize the AtExitManager on the main application thread to avoid
  // asserts and possible memory leaks.
  base::AtExitManager at_exit_manager_;

  CefSettings settings_;
  CefBrowserSettings browser_defaults_;
  FilePath cache_path_;
  scoped_refptr<BrowserRequestContext> request_context_;
  scoped_ptr<DOMStorageContext> storage_context_;

  // Map of browsers that currently exist.
  BrowserList browserlist_;

  // Used for assigning unique IDs to browser instances.
  int next_browser_id_;
};

// Global context object pointer
extern CefRefPtr<CefContext> _Context;

#endif // _CEF_CONTEXT_H