#include "UE5Bridge.h"

namespace EncryptionKeys
{
	static const int TIMESTAMP_ADDER = 123456789; 
	static const auto MD5_HASH_KEY = "a25ea98b37d4efb661f2cf679e1a9bfd165060a7"; // md5 hash of "ue5-laravel-bridge"
}

struct AnotherFenceToGetOverEncryption
{
	static FString Encrypt(const FString& InputString, const FString& Key)
	{
		TArray<uint8> EncryptedData;
		int32 KeyIndex = 0;

		for (int32 i = 0; i < InputString.Len(); ++i)
		{
			uint8 EncryptedByte = InputString[i] ^ Key[KeyIndex];
			EncryptedData.Add(EncryptedByte);

			KeyIndex = (KeyIndex + 1) % Key.Len();
		}

		return FBase64::Encode(EncryptedData.GetData(), EncryptedData.Num());
	}

	static FString Decrypt(const FString& EncodedString, const FString& Key)
	{
		TArray<uint8> DecodedData;
		FBase64::Decode(EncodedString, DecodedData);

		FString DecryptedString;
		int32 KeyIndex = 0;

		for (int32 i = 0; i < DecodedData.Num(); ++i)
		{
			uint8 const DecryptedByte = DecodedData[i] ^ Key[KeyIndex];
			DecryptedString.AppendChar(DecryptedByte);

			KeyIndex = (KeyIndex + 1) % Key.Len();
		}

		return DecryptedString;
	}
};

FString UUE5Bridge::GetCurrentTime() {
	FDateTime const Time = FDateTime::UtcNow();
	FString CurrentTime = Time.ToString();
	return CurrentTime;
}

FString UUE5Bridge::GenerateDynamicTokenFromTime(FString IncomingTime) {
	FDateTime ParsedDateTime;
	if (FDateTime::Parse(IncomingTime, ParsedDateTime))
	{
		FDateTime const CurrentTime = ParsedDateTime;
		FDateTime const DynamicTime = CurrentTime + FTimespan::FromSeconds(EncryptionKeys::TIMESTAMP_ADDER); 
		FString DynamicToken = DynamicTime.ToString();
		return DynamicToken;
	}

	// Error ?
	return "";
}

FString UUE5Bridge::HashToken(const FString& DynamicToken) {
	FString EncryptedString = AnotherFenceToGetOverEncryption::Encrypt(DynamicToken, EncryptionKeys::MD5_HASH_KEY);
	return EncryptedString;
}

TMap<FString, FString> UUE5Bridge::GetHashedToken(const TMap<FString, FString>& Params) {
	TMap<FString, FString> newParams = Params;
	FString const currentTime = GetCurrentTime();
	newParams.Add("timestamp", currentTime);
	FString const timeToken = GenerateDynamicTokenFromTime(currentTime);
	FString const csrfToken = HashToken(timeToken);
	newParams.Add("token", csrfToken);
	return newParams;
}