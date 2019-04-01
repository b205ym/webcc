#ifndef WEBCC_HTTP_REQUEST_BUILDER_H_
#define WEBCC_HTTP_REQUEST_BUILDER_H_

#include <string>
#include <vector>

#include "webcc/http_request.h"

namespace webcc {

class HttpRequestBuilder {
public:
  explicit HttpRequestBuilder(const std::string& method = "")
      : method_(method) {
  }

  // Build the request.
  HttpRequestPtr Build();

  HttpRequestPtr operator()() {
    return Build();
  }

  HttpRequestBuilder& Get()     { return Method(http::methods::kGet);     }
  HttpRequestBuilder& Head()    { return Method(http::methods::kHead);    }
  HttpRequestBuilder& Post()    { return Method(http::methods::kPost);    }
  HttpRequestBuilder& Put()     { return Method(http::methods::kPut);     }
  HttpRequestBuilder& Delete()  { return Method(http::methods::kDelete);  }
  HttpRequestBuilder& Patch()   { return Method(http::methods::kPatch);   }

  // NOTE:
  // The naming convention doesn't follow Google C++ Style for
  // consistency and simplicity.

  HttpRequestBuilder& Method(const std::string& method) {
    method_ = method;
    return *this;
  }

  HttpRequestBuilder& Url(const std::string& url) {
    url_ = url;
    return *this;
  }

  HttpRequestBuilder& Parameter(const std::string& key,
                                const std::string& value) {
    parameters_.push_back(key);
    parameters_.push_back(value);
    return *this;
  }

  HttpRequestBuilder& Data(const std::string& data) {
    data_ = data;
    return *this;
  }

  HttpRequestBuilder& Data(std::string&& data) {
    data_ = std::move(data);
    return *this;
  }

  HttpRequestBuilder& Json(bool json = true) {
    json_ = json;
    return *this;
  }

  // Upload a file with its path.
  // TODO: UNICODE file path.
  HttpRequestBuilder& File(const std::string& name,
                           const std::string& file_name,
                           const std::string& file_path,
                           const std::string& content_type = "");

  // Upload a file with its data.
  HttpRequestBuilder& FileData(const std::string& name,
                               const std::string& file_name,
                               std::string&& file_data,
                               const std::string& content_type = "") {
    files_.push_back({name, file_name, file_data, content_type});
    return *this;
  }

  HttpRequestBuilder& Gzip(bool gzip = true) {
    gzip_ = gzip;
    return *this;
  }

  HttpRequestBuilder& Header(const std::string& key,
                             const std::string& value) {
    headers_.push_back(key);
    headers_.push_back(value);
    return *this;
  }

  HttpRequestBuilder& KeepAlive(bool keep_alive) {
    keep_alive_ = keep_alive;
    return *this;
  }

  HttpRequestBuilder& Auth(const std::string& type,
                           const std::string& credentials);

  HttpRequestBuilder& AuthBasic(const std::string& login,
                                const std::string& password);

private:
  void SetContent(HttpRequestPtr request, std::string&& data);

  void CreateFormData(std::string* data, const std::string& boundary);
  
private:
  std::string method_;

  std::string url_;

  // URL query parameters.
  std::vector<std::string> parameters_;

  // Data to send in the body of the request.
  std::string data_;

  // Is the data to send a JSON string?
  bool json_ = false;

  // A file to upload.
  // Examples:
  //   { "images", "example.jpg", "BinaryData", "image/jpeg" }
  //   { "file", "report.csv", "BinaryData", "" }
  struct UploadFile {
    std::string name;
    std::string file_name;
    std::string file_data;  // Binary file data
    std::string content_type;
  };

  // Files to upload for a POST (or PUT?) request.
  std::vector<UploadFile> files_;

  // Compress the request content.
  // NOTE: Most servers don't support compressed requests.
  // Even the requests module from Python doesn't have a built-in support.
  // See: https://github.com/kennethreitz/requests/issues/1753
  bool gzip_ = false;

  // Additional request headers.
  std::vector<std::string> headers_;

  // Persistent connection.
  bool keep_alive_ = true;
};

}  // namespace webcc

#endif  // WEBCC_HTTP_REQUEST_BUILDER_H_
