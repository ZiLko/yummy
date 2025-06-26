#ifndef GEODE_IS_IOS
#include "Includes.hpp"

#include <geode.custom-keybinds/include/Keybinds.hpp>

$on_mod(Loaded) {    
    using namespace keybinds;
    
    BindManager::get()->registerBindable({
        "consume-decoration"_spr,
        "Eat Decoration!",
        "Hold to eat the level decoration!",
        { Keybind::create(KEY_E, Modifier::None) },
        "Yummy",
        false
    });
};
#endif