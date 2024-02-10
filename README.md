# UE5-Laravel Bridge
A robust CSRF inspired token designed to secure Laravel against MITM attacks by authenticating HTTP requests from Unreal Engine 5 (UE5) using CSRF-inspired tokens.

## Overview

The `ue5-laravel-bridge` utilizes a token-based authentication mechanism to secure interactions between UE5 clients and Laravel servers. This approach prevents unauthorized access and enhances security by ensuring requests are authenticated and originate from trusted sources.

## Features

- CSRF-inspired token authentication to secure HTTP requests.
- Timestamp-based dynamic token generation to prevent replay attacks.
- Encryption to ensure token integrity and confidentiality.
- Seamless integration with UE5 projects and Laravel applications.

## Getting Started

### Prerequisites

- An Unreal Engine 5 project.
- A Laravel-based application.

### Setup Instructions

#### On the Unreal Engine 5 Side

1. **Add Headers to HTTP Requests**: Modify your HTTP request logic in UE5 to include custom headers for user agent, current timestamp, and the CSRF token. Insert the following code snippet into your HTTP request setup:

```cpp
#if WITH_EDITOR
    Request->SetHeader(TEXT("User-Agent"), TEXT("UE5-Editor"));
#else
    Request->SetHeader(TEXT("User-Agent"), TEXT("UE5-Client"));
#endif

FString const currentTime = UUE5Bridge::GetCurrentTime();
Request->SetHeader(TEXT("X-UE5-Timestamp"), *currentTime);
FString const timeToken = UUE5Bridge::GenerateDynamicTokenFromTime(currentTime);
FString const csrfToken = UUE5Bridge::HashToken(timeToken);
Request->SetHeader(TEXT("X-UE5-Token"), *csrfToken);
```
#### On the Laravel Side

To validate requests from Unreal Engine 5 in your Laravel application, follow these steps:

1. **Add WebBridge.php to your Laravel Project**: Make sure your `WebBridge.php` is within your `App\Http\Controllers\` directory.


2. **Encryption Key and Timestamp Addition**: Ensure that the `$ENCRYPTION_KEY` and `$ADDITIONAL_TIME` values in your `WebBridge.php` match those specified in the Unreal Engine 5 implementation. These values are crucial for the encryption and decryption process to work correctly between the two platforms.

To use add to the beginning of your **API Controller** `CRUD` method.

```php
$WebBridge = new WebBridge();

if (!$WebBridge->VerifyRequest()) {
    throw new Exception('Unauthorized Request');
}
```

## Contributions

If you feel you can help improve this repository please feel free to submit PR Requests. Please update the readme if applicable.


## Issues or Bugs

Please submit a [new Issue](https://github.com/levizoesch/ue5-laravel-bridge/issues/new) if you need help, or have bugs.
