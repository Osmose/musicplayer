#include "AyflyPlugin.h"
namespace chipmachine {
static ChipPlugin::RegisterMe registerMe([](const std::string &configDir) -> std::shared_ptr<AyflyPlugin> { return std::make_shared<AyflyPlugin>(); });
}
