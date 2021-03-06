
#include "OpenMPTPlugin.h"

#include "libopenmpt/libopenmpt.h"
#include "libopenmpt/libopenmpt_stream_callbacks_file.h"

#include "../../chipplayer.h"

#include <coreutils/file.h>
#include <coreutils/settings.h>
#include <coreutils/utils.h>
#include <set>
#include <unordered_map>

using namespace std;

namespace musix {

class OpenMPTPlayer : public ChipPlayer {
public:
    OpenMPTPlayer(vector<uint8_t> data) {

        uint8_t* ptr = &data[0];
        if(memcmp(ptr + 1080, "FLT", 3) == 0 ||
           memcmp(ptr + 1080, "EXO", 3) == 0)
            throw player_exception("Can not play Startrekker module");

        mod = openmpt_module_create_from_memory(&data[0], data.size(), nullptr,
                                                nullptr, nullptr);

        if(!mod)
            throw player_exception("Could not load module");

        // if(loopmode)
        openmpt_module_set_repeat_count(mod, 99);

        auto length = openmpt_module_get_duration_seconds(mod);
        auto songs = openmpt_module_get_num_subsongs(mod);

        auto keys = openmpt_module_get_metadata_keys(mod);

        auto title = openmpt_module_get_metadata(mod, "title");
        auto artist = openmpt_module_get_metadata(mod, "artist");
        auto tracker = openmpt_module_get_metadata(mod, "tracker");
        auto type = openmpt_module_get_metadata(mod, "type");
        auto message = openmpt_module_get_metadata(mod, "message");
        auto type_long = openmpt_module_get_metadata(mod, "type_long");

        if(strcmp(type, "mod") == 0)
            openmpt_module_set_render_param(
                mod, OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH, 1);
        else
            openmpt_module_set_render_param(
                mod, OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH, 0);

        auto& Settings = utils::Settings::getGroup("openmpt");
        double separation = Settings.get<double>("separation", 100.0);
        openmpt_module_set_render_param(
            mod, OPENMPT_MODULE_RENDER_STEREOSEPARATION_PERCENT, separation);

        auto p = utils::split(string(type_long), " / ");
        if(p.size() > 1)
            type_long = p[0].c_str();

        setMeta("title", title, "composer", artist, "message", message,
                "tracker", tracker, "format", type_long, "type", type, "songs",
                songs, "length", length);
    }
    ~OpenMPTPlayer() override {
        if(mod)
            openmpt_module_destroy(mod);
    }

    virtual int getSamples(int16_t* target, int noSamples) override {
        auto len = openmpt_module_read_interleaved_stereo(
            mod, 44100, noSamples / 2, target);
        return len * 2;
    }

    virtual bool seekTo(int song, int seconds) override {
        if(mod) {
            if(song >= 0)
                openmpt_module_select_subsong(mod, song);
            else
                openmpt_module_set_position_seconds(mod, seconds);
            return true;
        }
        return false;
    }

private:
    openmpt_module* mod;
};

// static const set<string> supported_ext { "mod", "xm", "s3m" , "oct", /*"okt",
// "okta", sucks here, use UADE */ "it", "ft", "far", "ult", "669", "dmf", "mdl",
// "stm", "okt", "gdm", "mt2", "mtm", "j2b", "imf", "ptm", "ams" };

bool OpenMPTPlugin::canHandle(const std::string& n) {
    auto name = utils::toLower(n);
    auto ext = utils::path_extension(name);
    if(ext == "gz")
        return false;
    if(name.find("2fstk.") != string::npos ||
       name.find("2fmod.") != string::npos) {
        return true;
    }
    if(ext == "ft")
        return true;
    if(ext == "rns" || ext == "dtm")
        return false;
    return openmpt_is_extension_supported(ext.c_str());
    // return supported_ext.count(utils::path_extension(name)) > 0;
}

ChipPlayer* OpenMPTPlugin::fromFile(const std::string& fileName) {
    utils::File file{fileName};
    try {
        return new OpenMPTPlayer{file.readAll()};
    } catch(player_exception& e) {
        return nullptr;
    }
};

} // namespace musix
