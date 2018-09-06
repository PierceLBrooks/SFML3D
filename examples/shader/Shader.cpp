
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Effect.hpp"
#include <SFML3D/Graphics.hpp>
#include <vector>
#include <cmath>


const sf3d::Font* Effect::s_font = NULL;

////////////////////////////////////////////////////////////
// "Pixelate" fragment shader
////////////////////////////////////////////////////////////
class Pixelate : public Effect
{
public :

    Pixelate() :
    Effect("pixelate")
    {
    }

    bool onLoad()
    {
        // Load the texture and initialize the sprite
        if (!m_texture.loadFromFile("resources/background.jpg"))
            return false;
        m_sprite.setTexture(m_texture);

        // Load the shader
        if (!m_shader.loadFromFile("resources/pixelate.vert", "resources/pixelate.frag"))
            return false;

        return true;
    }

    void onUpdate(float, float x, float y)
    {
        m_shader.setParameter("pixel_threshold", (x + y) / 30);
    }

    void onDraw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
    {
        states.shader = &m_shader;
        target.draw(m_sprite, states);
    }

private:

    sf3d::Texture m_texture;
    sf3d::Sprite m_sprite;
    sf3d::Shader m_shader;
};


////////////////////////////////////////////////////////////
// "Wave" vertex shader + "blur" fragment shader
////////////////////////////////////////////////////////////
class WaveBlur : public Effect
{
public :

    WaveBlur() :
    Effect("wave + blur")
    {
    }

    bool onLoad()
    {
        // Create the text
        m_text.setString("Praesent suscipit augue in velit pulvinar hendrerit varius purus aliquam.\n"
                         "Mauris mi odio, bibendum quis fringilla a, laoreet vel orci. Proin vitae vulputate tortor.\n"
                         "Praesent cursus ultrices justo, ut feugiat ante vehicula quis.\n"
                         "Donec fringilla scelerisque mauris et viverra.\n"
                         "Maecenas adipiscing ornare scelerisque. Nullam at libero elit.\n"
                         "Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.\n"
                         "Nullam leo urna, tincidunt id semper eget, ultricies sed mi.\n"
                         "Morbi mauris massa, commodo id dignissim vel, lobortis et elit.\n"
                         "Fusce vel libero sed neque scelerisque venenatis.\n"
                         "Integer mattis tincidunt quam vitae iaculis.\n"
                         "Vivamus fringilla sem non velit venenatis fermentum.\n"
                         "Vivamus varius tincidunt nisi id vehicula.\n"
                         "Integer ullamcorper, enim vitae euismod rutrum, massa nisl semper ipsum,\n"
                         "vestibulum sodales sem ante in massa.\n"
                         "Vestibulum in augue non felis convallis viverra.\n"
                         "Mauris ultricies dolor sed massa convallis sed aliquet augue fringilla.\n"
                         "Duis erat eros, porta in accumsan in, blandit quis sem.\n"
                         "In hac habitasse platea dictumst. Etiam fringilla est id odio dapibus sit amet semper dui laoreet.\n");
        m_text.setFont(getFont());
        m_text.setCharacterSize(22);
        m_text.setPosition(30, 20);

        // Load the shader
        if (!m_shader.loadFromFile("resources/wave.vert", "resources/blur.frag"))
            return false;

        return true;
    }

    void onUpdate(float time, float x, float y)
    {
        m_shader.setParameter("wave_phase", time);
        m_shader.setParameter("wave_amplitude", x * 40, y * 40);
        m_shader.setParameter("blur_radius", (x + y) * 0.008f);
    }

    void onDraw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
    {
        states.shader = &m_shader;
        target.draw(m_text, states);
    }

private:

    sf3d::Text m_text;
    sf3d::Shader m_shader;
};


////////////////////////////////////////////////////////////
// "Storm" vertex shader + "blink" fragment shader
////////////////////////////////////////////////////////////
class StormBlink : public Effect
{
public :

    StormBlink() :
    Effect("storm + blink")
    {
    }

    bool onLoad()
    {
        // Create the points
        m_points.setPrimitiveType(sf3d::Points);
        for (int i = 0; i < 40000; ++i)
        {
            float x = static_cast<float>(std::rand() % 800);
            float y = static_cast<float>(std::rand() % 600);
            sf3d::Uint8 r = std::rand() % 255;
            sf3d::Uint8 g = std::rand() % 255;
            sf3d::Uint8 b = std::rand() % 255;
            m_points.append(sf3d::Vertex(sf3d::Vector2f(x, y), sf3d::Color(r, g, b)));
        }

        // Load the shader
        if (!m_shader.loadFromFile("resources/storm.vert", "resources/blink.frag"))
            return false;

        // Generate a 1D texture full of random numbers
        sf3d::Uint8 random_numbers[4000];
        for (int i = 0; i < 4000; ++i)
            random_numbers[i] = std::rand() % 255;

        if (!m_random_texture.create(1000))
            return false;
        m_random_texture.update(random_numbers);
        m_shader.setParameter("random_texture", m_random_texture);

        return true;
    }

    void onUpdate(float time, float x, float y)
    {
        float radius = 200 + std::cos(time) * 150;
        m_shader.setParameter("storm_position", x * 800, y * 600);
        m_shader.setParameter("storm_inner_radius", radius / 3);
        m_shader.setParameter("storm_total_radius", radius);
        m_shader.setParameter("blink_alpha", 0.5f + std::cos(time * 3) * 0.25f);
    }

    void onDraw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
    {
        states.shader = &m_shader;
        target.draw(m_points, states);
    }

private:

    sf3d::VertexContainer m_points;
    sf3d::Shader m_shader;
    sf3d::Texture m_random_texture;
};


////////////////////////////////////////////////////////////
// "Edge" post-effect fragment shader
////////////////////////////////////////////////////////////
class Edge : public Effect
{
public :

    Edge() :
    Effect("edge post-effect")
    {
    }

    bool onLoad()
    {
        // Create the off-screen surface
        if (!m_surface.create(800, 600))
            return false;
        m_surface.setSmooth(true);

        // Load the textures
        if (!m_backgroundTexture.loadFromFile("resources/sfml3d.png"))
            return false;
        m_backgroundTexture.setSmooth(true);
        if (!m_entityTexture.loadFromFile("resources/devices.png"))
            return false;
        m_entityTexture.setSmooth(true);

        // Initialize the background sprite
        m_backgroundSprite.setTexture(m_backgroundTexture);
        m_backgroundSprite.setPosition(135, 100);

        // Load the moving entities
        for (int i = 0; i < 6; ++i)
        {
            sf3d::Sprite entity(m_entityTexture, sf3d::IntRect(96 * i, 0, 96, 96));
            m_entities.push_back(entity);
        }

        // Load the shader
        if (!m_shader.loadFromFile("resources/edge.vert", "resources/edge.frag"))
            return false;

        return true;
    }

    void onUpdate(float time, float x, float y)
    {
        m_shader.setParameter("edge_threshold", 1 - (x + y) / 2);

        // Update the position of the moving entities
        for (std::size_t i = 0; i < m_entities.size(); ++i)
        {
            sf3d::Vector2f position;
            position.x = std::cos(0.25f * (time * i + (m_entities.size() - i))) * 300 + 350;
            position.y = std::sin(0.25f * (time * (m_entities.size() - i) + i)) * 200 + 250;
            m_entities[i].setPosition(position);
        }

        // Render the updated scene to the off-screen surface
        m_surface.clear(sf3d::Color::White);
        m_surface.draw(m_backgroundSprite);
        for (std::size_t i = 0; i < m_entities.size(); ++i)
            m_surface.draw(m_entities[i]);
        m_surface.display();
    }

    void onDraw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
    {
        states.shader = &m_shader;
        target.draw(sf3d::Sprite(m_surface.getTexture()), states);
    }

private:

    sf3d::RenderTexture m_surface;
    sf3d::Texture m_backgroundTexture;
    sf3d::Texture m_entityTexture;
    sf3d::Sprite m_backgroundSprite;
    std::vector<sf3d::Sprite> m_entities;
    sf3d::Shader m_shader;
};


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf3d::RenderWindow window(sf3d::VideoMode(800, 600), "SFML3D Shader");
    window.setVerticalSyncEnabled(true);

    // Load the application font and pass it to the Effect class
    sf3d::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;
    Effect::setFont(font);

    // Create the effects
    std::vector<Effect*> effects;
    effects.push_back(new Pixelate);
    effects.push_back(new WaveBlur);
    effects.push_back(new StormBlink);
    effects.push_back(new Edge);
    std::size_t current = 0;

    // Initialize them
    for (std::size_t i = 0; i < effects.size(); ++i)
        effects[i]->load();

    // Create the messages background
    sf3d::Texture textBackgroundTexture;
    if (!textBackgroundTexture.loadFromFile("resources/text-background.png"))
        return EXIT_FAILURE;
    sf3d::Sprite textBackground(textBackgroundTexture);
    textBackground.setPosition(0, 520);
    textBackground.setColor(sf3d::Color(255, 255, 255, 200));

    // Create the description text
    sf3d::Text description("Current effect: " + effects[current]->getName(), font, 20);
    description.setPosition(10, 530);
    description.setColor(sf3d::Color(80, 80, 80));

    // Create the instructions text
    sf3d::Text instructions("Press left and right arrows to change the current shader", font, 20);
    instructions.setPosition(280, 555);
    instructions.setColor(sf3d::Color(80, 80, 80));

    // Start the game loop
    sf3d::Clock clock;
    while (window.isOpen())
    {
        // Process events
        sf3d::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf3d::Event::Closed)
                window.close();

            if (event.type == sf3d::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    // Escape key: exit
                    case sf3d::Keyboard::Escape:
                        window.close();
                        break;

                    // Left arrow key: previous shader
                    case sf3d::Keyboard::Left:
                        if (current == 0)
                            current = effects.size() - 1;
                        else
                            current--;
                        description.setString("Current effect: " + effects[current]->getName());
                        break;

                    // Right arrow key: next shader
                    case sf3d::Keyboard::Right:
                        if (current == effects.size() - 1)
                            current = 0;
                        else
                            current++;
                        description.setString("Current effect: " + effects[current]->getName());
                        break;

                    default:
                        break;
                }
            }
        }

        // Update the current example
        float x = static_cast<float>(sf3d::Mouse::getPosition(window).x) / window.getSize().x;
        float y = static_cast<float>(sf3d::Mouse::getPosition(window).y) / window.getSize().y;
        effects[current]->update(clock.getElapsedTime().asSeconds(), x, y);

        // Clear the window
        window.clear(sf3d::Color(255, 128, 0));

        // Draw the current example
        window.draw(*effects[current]);

        // Draw the text
        window.draw(textBackground);
        window.draw(instructions);
        window.draw(description);

        // Finally, display the rendered frame on screen
        window.display();
    }

    // delete the effects
    for (std::size_t i = 0; i < effects.size(); ++i)
        delete effects[i];

    return EXIT_SUCCESS;
}
