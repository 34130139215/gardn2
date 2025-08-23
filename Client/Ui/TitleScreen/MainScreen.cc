#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Ui/Button.hh>
#include <Client/Ui/Choose.hh>
#include <Client/Ui/Container.hh>
#include <Client/Ui/DynamicText.hh>
#include <Client/Ui/TextInput.hh>

#include <Client/Debug.hh>
#include <Client/DOM.hh>
#include <Client/Game.hh>

#include <Shared/Config.hh>

using namespace Ui;

Element *Ui::make_title_input_box() {
    Ui::Element *title = new Ui::VContainer({
        new Ui::Element(0, 60),
        new Ui::Choose(
            new Ui::StaticText(40, "Connecting..."),
            new Ui::VContainer({
                new Ui::StaticText(20, "This pretty little flower is called..."),
                new Ui::HContainer({
                    new Ui::TextInput(Game::nickname, 400, 50, MAX_NAME_LENGTH, {
                        .line_width = 5,
                        .round_radius = 5,
                        .animate = [](Element *elt, Renderer &ctx) { 
                            ctx.translate(0, (elt->animation - 1) * ctx.height * 0.6);
                        },
                        .should_render = [](){
                            return !Game::in_game() && Game::transition_circle <= 0;
                        }
                    }),
                    new Ui::Button(110, 36, 
                        new Ui::StaticText(25, "Spawn"), 
                        [](Element *elt, uint8_t e){ if (e == Ui::kClick) Game::spawn_in(); },
                        [](){ return Game::in_game() != 0; },
                        { .fill = 0xff1dd129, .line_width = 5, .round_radius = 3 }
                    )
                }, 0, 10,{}),
                new Ui::StaticText(14, "(or press ENTER to spawn)")
            }, 10, 5, { .animate = [](Element *elt, Renderer &ctx) {
                ctx.translate(0, (elt->animation - 1) * ctx.height);
            } }),
            [](){ return Game::socket.ready; }
        ),
        new Ui::Element(0,20),
    }, 0, 0, { .animate = [](Element *elt, Renderer &ctx){}, .should_render = [](){ return Game::should_render_title_ui(); } });
    return title;
}

Element *Ui::make_title_info_box() {
    Element *elt = new Ui::Choose(
        new Ui::Choose(
            new Ui::VContainer({
                new Ui::StaticText(35, "How to play"),
                new Ui::Element(0,5),
                new Ui::StaticText(16, "Use mouse to move"),
                new Ui::StaticText(16, "Right click to attack"),
                new Ui::StaticText(16, "Left click to defend")
            }, 0, 5, { .no_animation = 1 }),
            new Ui::VContainer({
                new Ui::StaticText(35, "How to play"),
                new Ui::Element(0,5),
                new Ui::StaticText(16, "Use WASD or arrow keys to move"),
                new Ui::StaticText(16, "SPACE to attack"),
                new Ui::StaticText(16, "SHIFT to defend")
            }, 0, 5, { .no_animation = 1 }),
            [](){
                return Input::keyboard_movement;
            }, { .no_polling = 1 }
        ),
        new Ui::VContainer({
            new Ui::HContainer({
                new Ui::DynamicText(16, [](){
                    return std::format("You will respawn at level {}", Game::respawn_level);
                }, { .fill = 0xffffffff, .no_animation = 1 }),
                new Ui::StaticText(16, " with:", {
                    .fill = 0xffffffff,
                    .should_render = [](){
                        if (!Game::simulation.ent_exists(Game::camera_id)) return false;
                        return Game::simulation.get_ent(Game::camera_id).inventory[0] > PetalID::kBasic;
                    }
                })
            }, 0, 0),
            new Ui::HContainer(
                Ui::make_range(0, MAX_SLOT_COUNT, [](uint32_t i){ return (Element *) (new Ui::TitlePetalSlot(i)); })
            , 0, 10),
            new Ui::HContainer({
                Ui::make_range(MAX_SLOT_COUNT, 2*MAX_SLOT_COUNT, [](uint32_t i){ return (Element *) (new Ui::TitlePetalSlot(i)); })
            }, 0, 10)
        }, 0, 10, { .no_animation = 1 }),
        [](){
            return Game::respawn_level > 1 ? 1 : 0;
        }
    );
    elt->x = 0;
    elt->y = 270;
    return elt;
}


Element *Ui::make_panel_buttons() {
   Element *elt = new Ui::HContainer({
        new Ui::Button(100, 35, 
            new Ui::StaticText(16, "Settings"), 
            [](Element *elt, uint8_t e){ if (e == Ui::kClick) {
                if (Ui::panel_open != Panel::kSettings) {
                    Ui::panel_open = Panel::kSettings;
                    Element *pg = Ui::Panel::settings;
                    pg->x = elt->screen_x / Ui::scale - pg->width / 2;
                    pg->y = -(elt->height + 20);
                    if (pg->x < 10) 
                        pg->x = 10;
                }
                else Ui::panel_open = Panel::kNone;
            } },
            [](){ return Ui::panel_open == Panel::kSettings; },
            { .fill = 0xff5a9fdb, .line_width = 5, .round_radius = 3 }
        ),
        new Ui::Button(100, 35, 
            new Ui::HContainer({
                new Ui::Element(0,0),
                new Ui::StaticText(16, "Petals"),
                new Ui::PetalsCollectedIndicator(20)
            }, 0, 10),
            [](Element *elt, uint8_t e){ if (e == Ui::kClick) {
                if (Ui::panel_open != Panel::kPetals) {
                    Ui::panel_open = Panel::kPetals;
                    Element *pg = Ui::Panel::petal_gallery;
                    pg->x = elt->screen_x / Ui::scale - pg->width / 2;
                    pg->y = -(elt->height + 20);
                    if (pg->x < 10) 
                        pg->x = 10;
                }
                else Ui::panel_open = Panel::kNone;
            } },
            [](){ return Ui::panel_open == Panel::kPetals; },
            { .fill = 0xff5a9fdb, .line_width = 5, .round_radius = 3 }
        ),
        new Ui::Button(100, 35, 
            new Ui::StaticText(16, "Mobs"), 
            [](Element *elt, uint8_t e){ if (e == Ui::kClick) {
                if (Ui::panel_open != Panel::kMobs) {
                    Ui::panel_open = Panel::kMobs;
                    Element *pg = Ui::Panel::mob_gallery;
                    pg->x = elt->screen_x / Ui::scale - pg->width / 2;
                    pg->y = -(elt->height + 20);
                    if (pg->x < 10) 
                        pg->x = 10;
                }
                else Ui::panel_open = Panel::kNone;
            } },
            [](){ return Ui::panel_open == Panel::kMobs; },
            { .fill = 0xff5a9fdb, .line_width = 5, .round_radius = 3 }
        ),
        new Ui::Button(120, 35, 
            new Ui::StaticText(16, "Changelog"), 
            [](Element *elt, uint8_t e){ if (e == Ui::kClick) {
                if (Ui::panel_open != Panel::kChangelog) {
                    Ui::panel_open = Panel::kChangelog;
                    Element *pg = Ui::Panel::changelog;
                    pg->x = elt->screen_x / Ui::scale - pg->width / 2;
                    pg->y = -(elt->height + 20);
                    if (pg->x < 10) 
                        pg->x = 10;
                }
                else Ui::panel_open = Panel::kNone;
            } },
            [](){ return Ui::panel_open == Panel::kChangelog; },
            { .fill = 0xff5a9fdb, .line_width = 5, .round_radius = 3 }
        ),
   }, 10, 10, { .should_render = [](){ return Game::should_render_title_ui(); }, .h_justify = Style::Left, .v_justify = Style::Bottom });
   return elt;
}

Element *Ui::make_debug_stats() {
    Element *elt = new Ui::VContainer({
        new Ui::DynamicText(12, [](){
            float pxls = 0;
            for (Renderer *r : Renderer::renderers) pxls += r->width * r->height;
            return std::format("{} ctxs - {} pxls - {:.0f}x{:.0f}", Renderer::renderers.size(), format_score(pxls), Ui::window_width, Ui::window_height);
        }, { .fill = 0xffffffff, .h_justify = Style::Right }),
        new Ui::DynamicText(12, [](){
            if (Debug::frame_times.size() == 0) return std::string{"Failed to show debug stats"};
            float min_dt = Debug::tick_times[0];
            float avg_dt = min_dt;
            float max_dt = min_dt;
            for (uint32_t i = 1; i < Debug::tick_times.size(); ++i) {
                float const frame_time = Debug::tick_times[i];
                if (min_dt > frame_time) min_dt = frame_time;
                else if (max_dt < frame_time) max_dt = frame_time;
                avg_dt += frame_time;
            }
            avg_dt /= Debug::tick_times.size();
            return std::format("tick: {:.1f}/{:.1f}/{:.1f} ms (min/avg/max)", min_dt, avg_dt, max_dt, 1000 / Ui::dt);
        }, { .fill = 0xffffffff, .h_justify = Style::Right }),
        new Ui::DynamicText(12, [](){
            if (Debug::frame_times.size() == 0) return std::string{"Failed to show debug stats"};
            float min_dt = Debug::frame_times[0];
            float avg_dt = min_dt;
            float max_dt = min_dt;
            for (uint32_t i = 1; i < Debug::frame_times.size(); ++i) {
                float const frame_time = Debug::frame_times[i];
                if (min_dt > frame_time) min_dt = frame_time;
                else if (max_dt < frame_time) max_dt = frame_time;
                avg_dt += frame_time;
            }
            avg_dt /= Debug::frame_times.size();
            return std::format("frame: {:.1f}/{:.1f}/{:.1f} ms (min/avg/max) - {:.1f} fps", min_dt, avg_dt, max_dt, 1000 / Ui::dt);
        }, { .fill = 0xffffffff, .h_justify = Style::Right })
    }, 5, 5, { .should_render = [](){ return Game::show_debug; }, .h_justify = Style::Right, .v_justify = Style::Bottom, .no_animation = 1 });
    return elt;
}

Element *Ui::make_discord_link_button() {
    Element *elt = new Ui::HContainer({
        new Ui::Button(50, 50, 
            new Ui::StaticIcon([](Element *elt, Renderer &ctx){
                ctx.scale(elt->width / 20);
                ctx.set_fill(0xfff1f1f1);

                ctx.begin_path();
                ctx.move_to(8.26, -1.80);
                ctx.bcurve_to(7.66, -2.00, 7.04, -2.17, 6.39, -2.29);
                ctx.bcurve_to(6.27, -2.06, 6.13, -1.77, 6.05, -1.55);
                ctx.bcurve_to(5.40, -1.67, 4.74, -1.67, 4.09, -1.55);
                ctx.bcurve_to(4.01, -1.77, 3.87, -2.06, 3.75, -2.29);
                ctx.bcurve_to(3.10, -2.17, 2.48, -2.00, 1.88, -1.80);
                ctx.bcurve_to(0.90, -0.41, 0.58, 0.96, 0.70, 2.30);
                ctx.bcurve_to(1.56, 2.91, 2.41, 3.32, 3.26, 3.58);
                ctx.bcurve_to(3.46, 3.29, 3.64, 2.99, 3.81, 2.66);
                ctx.bcurve_to(3.51, 2.57, 3.21, 2.47, 2.92, 2.33);
                ctx.bcurve_to(3.00, 2.26, 3.07, 2.18, 3.15, 2.11);
                ctx.bcurve_to(4.52, 2.72, 5.90, 2.72, 7.26, 2.11);
                ctx.bcurve_to(7.34, 2.18, 7.41, 2.26, 7.49, 2.33);
                ctx.bcurve_to(7.20, 2.47, 6.90, 2.57, 6.60, 2.66);
                ctx.bcurve_to(6.78, 2.99, 6.96, 3.29, 7.16, 3.58);
                ctx.bcurve_to(8.01, 3.32, 8.86, 2.91, 9.72, 2.30);
                ctx.bcurve_to(9.86, 0.95, 9.50, -0.41, 8.26, -1.80);
                ctx.close_path();

                ctx.move_to(4.06, 1.58);
                ctx.bcurve_to(3.69, 1.58, 3.40, 1.21, 3.40, 0.74);
                ctx.bcurve_to(3.40, 0.27, 3.69, -0.10, 4.06, -0.10);
                ctx.bcurve_to(4.43, -0.10, 4.72, 0.27, 4.72, 0.74);
                ctx.bcurve_to(4.72, 1.21, 4.43, 1.58, 4.06, 1.58);
                ctx.close_path();

                ctx.move_to(6.36, 1.58);
                ctx.bcurve_to(5.99, 1.58, 5.70, 1.21, 5.70, 0.74);
                ctx.bcurve_to(5.70, 0.27, 5.99, -0.10, 6.36, -0.10);
                ctx.bcurve_to(6.73, -0.10, 7.02, 0.27, 7.02, 0.74);
                ctx.bcurve_to(7.02, 1.21, 6.73, 1.58, 6.36, 1.58);
                ctx.close_path();

                ctx.fill();
            }, 40, 40),
            [](Element *elt, uint8_t e){
                if (e == Ui::kClick) DOM::open_page("https://discord.gg/NkcStwd5tG");
            },
            nullptr,
            { .fill = 0xff5865F2, .line_width = 4, .round_radius = 4 }
        )
    }, 10, 10, { .h_justify = Style::Right, .v_justify = Style::Top });
    return elt;
}
