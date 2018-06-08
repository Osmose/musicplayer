#include <string>

#include <audioplayer/audioplayer.h>
#include <coreutils/log.h>
#include <coreutils/utils.h>

#include "plugins/plugins.h"

using musix::ChipPlayer;
using musix::ChipPlugin;

typedef struct musix::ChipPlayer ChipPlayer;

extern "C" {
    void init(char* dataDir) {
        logging::setLevel(logging::Level::Verbose);

        std::string strDataDir(dataDir);
        ChipPlugin::createPlugins(strDataDir);
    }

    char* getMeta(ChipPlayer* player, char* metaName) {
        std::string strMetaName(metaName);
        std::string result = player->getMeta(strMetaName);
        char* ret = new char[result.length() + 1];
        strcpy(ret, result.c_str());
        return ret;
    }

    int getMetaInt(ChipPlayer* player, char* metaName) {
        std::string strMetaName(metaName);
        return player->getMetaInt(strMetaName);
    }

    ChipPlayer* playerFor(char* name) {
        printf("FINDING PLAYER");
        ChipPlayer* player;
        for(auto &plugin : ChipPlugin::getPlugins()) {
            if(plugin->canHandle(name)) {
                printf("%s can handle", plugin->name().c_str());
                auto ptr = plugin->fromFile(name);
                if(ptr != nullptr) {
                    player = ptr;
                    break;
                }
            }
        }
        if(!player) {
            printf("No plugin could handle file\n");
            return nullptr;
        } else {
            printf("Players: %p\n", player);
        }

        return player;
    }

    void freePlayer(ChipPlayer* player) {
        delete player;
    }

    void play(ChipPlayer* player, int16_t *target, int size) {
        player->getSamples(target, size);
    }
}
