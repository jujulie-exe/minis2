#ifndef CONFIGKEYS_HPP
#define CONFIGKEYS_HPP

#include <string>

namespace ConfigKeys {

// System Section
constexpr const char *SYSTEM = "system";
namespace System {
constexpr const char *CHIPSET = "chipSet";
constexpr const char *GPIO = "gpio";
constexpr const char *RELAY_ACTIVE_LOW = "relay_active_low";
} // namespace System

// Camera Section
constexpr const char *CAMERA = "camera";
namespace Camera {
constexpr const char *WIDTH = "width";
constexpr const char *HEIGHT = "height";
constexpr const char *FORMAT = "format";
constexpr const char *SETTLE_TIME_MS = "settle_time_ms";

constexpr const char *EXPOSURE_MANUAL = "exposure_manual";
namespace ExposureManual {
constexpr const char *ENABLED = "enabled";
constexpr const char *VALUE = "value";
} // namespace ExposureManual

// Keys possibly used in code but not in current JSON or optional
constexpr const char *PATH = "path";
} // namespace Camera

// Output Section
constexpr const char *OUTPUT = "output";
namespace Output {
constexpr const char *SAVE_LOCALLY = "save_locally";
namespace SaveLocally {
constexpr const char *ENABLED = "enabled";
constexpr const char *DIRECTORY = "directory";
constexpr const char *PREFIX = "prefix";
} // namespace SaveLocally

constexpr const char *REMOTE_SERVER = "remote_server";
namespace RemoteServer {
constexpr const char *ENABLED = "enabled";
constexpr const char *IP = "ip";
constexpr const char *PORT = "port";
constexpr const char *ENDPOINT = "endpoint";
} // namespace RemoteServer
} // namespace Output

// Validation Section
constexpr const char *VALIDATION = "validation";
namespace Validation {
constexpr const char *FOCUS_CHECK = "focus_check";
constexpr const char *MIN_FOCUS_THRESHOLD = "min_focus_threshold";
} // namespace Validation

} // namespace ConfigKeys

#endif // CONFIGKEYS_HPP
