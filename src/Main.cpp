#ifndef GEODE_IS_IOS
#include "Includes.hpp"

#include <geode.custom-keybinds/include/Keybinds.hpp>

$on_mod(Loaded) {    
    using namespace keybinds;
    
    BindManager::get()->registerBindable({
        "backflip"_spr,
        "Do a Backflip!",
        "Throw a backflip",
        { Keybind::create(KEY_E, Modifier::None) },
        "My Mod/Awesome Tricks",
        false
    });
};
#endif