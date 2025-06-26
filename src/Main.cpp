#include "Includes.hpp"
#ifndef GEODE_IS_IOS
#include <geode.custom-keybinds/include/Keybinds.hpp>
#endif

$on_mod(Loaded) {    
    #ifndef GEODE_IS_IOS
    using namespace keybinds;
    
    BindManager::get()->registerBindable({
        "consume-decoration"_spr,
        "Eat Decoration!",
        "Hold to eat the level decoration!",
        { Keybind::create(KEY_E, Modifier::None) },
        "Yummy",
        false
    });
    #endif
    Vars::volumeSetting = Mod::get()->getSettingValue<int64_t>("volume");
    listenForSettingChanges("volume", [](int64_t newVolume) {
		Vars::volumeSetting = newVolume;
	});
};