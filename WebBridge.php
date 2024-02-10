namespace App\Http\Controllers;

use App\Http\Controllers\Controller;
use Carbon\Carbon;

class WebBridge extends Controller
{

    private string $ENCRYPTION_KEY;
    private int $ADDITIONAL_TIME;
    private string $DATETIME_FORMAT;

    public function __construct()
    {
        $this->ENCRYPTION_KEY = "a25ea98b37d4efb661f2cf679e1a9bfd165060a7"; // must match EncryptionKeys::MD5_HASH_KEY in UE5Bridge.cpp
        $this->ADDITIONAL_TIME = 123456789; // unix timestamp adder must match EncryptionKeys::TIMESTAMP_ADDER in UE5Bridge.cpp
        $this->DATETIME_FORMAT = 'Y.m.d-H.i.s'; // unreal engine time format.
    }

    public function VerifyRequest(): bool
    {

        $userAgent = Request()->header('USER_AGENT');
        $timeStamp = Request()->header('X-UE5-Timestamp');
        $ue5Token = Request()->header('X-UE5-Token');

        if (in_array($userAgent, ['UE5-Client', 'UE5-Server', 'UE5-Editor'])) {

            // Validate Timestamp and Generate Dynamic Token
            $carbonDate = Carbon::createFromFormat($this->DATETIME_FORMAT, $timeStamp);
            $carbonTimestamp = $carbonDate->timestamp;
            if (!$carbonDate) {
                return false;
            }

            // Add additional timestamp duration to current request timestamp
            $dynamicTime = $carbonTimestamp + $this->ADDITIONAL_TIME;

            // Convert back to UE5 Timestamp format for comparison
            $backToUETimestamp = Carbon::createFromTimestamp($dynamicTime, 'UTC')->format($this->DATETIME_FORMAT);

            // Expected encrypted token
            $encrypted = $this->encrypt($backToUETimestamp, $this->ENCRYPTION_KEY);

            if ($ue5Token !== $encrypted) {
                // Hash does not match
                return false;
            }

            // the incoming request can be
            // expected to be from the ue5 client.
            return true;
        }

        return false;
    }

    function encrypt($inputString, $key): string
    {
        $encryptedData = '';
        $keyLength = strlen($key);
        for ($i = 0; $i < strlen($inputString); $i++) {
            $encryptedByte = ord($inputString[$i]) ^ ord($key[$i % $keyLength]);
            $encryptedData .= chr($encryptedByte);
        }

        // Base64 encode the encrypted data
        return base64_encode($encryptedData);
    }

    function decrypt($encodedString, $key): string
    {
        $decodedData = base64_decode($encodedString);
        $decryptedString = '';
        $keyLength = strlen($key);
        for ($i = 0; $i < strlen($decodedData); $i++) {
            $decryptedByte = ord($decodedData[$i]) ^ ord($key[$i % $keyLength]);
            $decryptedString .= chr($decryptedByte);
        }

        return $decryptedString;
    }
}
