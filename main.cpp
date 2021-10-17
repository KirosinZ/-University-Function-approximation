#include <vector>
#include <random>
#include <functional>
#include <SFML/Graphics.hpp>

#define WND_W 2000
#define WND_H 1000
#define WND_PAD 100

const float H = 0.01f;
const float M0 = 1.0f;
const float M6 = 1.0f;
const float eps = std::numeric_limits<float>::epsilon();

const float H_Max = 1;
const float K_Max = 2500;

using range = std::vector<sf::Vector2f>;
using func = std::function<float(float)>;

float D5_sin_exact(float x)
{
    return cosf(x);
}

float D5_sin_calc(float x, float h)
{
    return (-sinf(x - 3 * h) + 5 * sinf(x - 2 * h) - 10 * sinf(x - h) + 10 * sinf(x) - 5 * sinf(x + h) + sinf(x + 2 * h))/ (powf(h, 5));
}

float Delta_exact(float h)
{
    return h/2*M6 + 16 * M0 * eps / powf(h, 5);
}

float Delta_calc(float h)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> rand(-20, 20);

    float x;
    float res = 0;
    for (int i = 0; i < 32; i++)
    {
        x = rand(gen);
        res = std::max(res, abs(D5_sin_calc(x, h) - D5_sin_exact(x)));
    }
    return res;
}

range arange(const func& f, float a, float b, float h)
{
    range r(ceilf((b - a) / h));
    int cnt = 0;
    for (auto iter = r.begin(); iter != r.end(); iter++, cnt++)
    {
        float cur = a + cnt * h;
        *iter = sf::Vector2f(cur, f(cur));
    }
    return r;
}

void plot(const range& rng1, sf::Color clr1, const range& rng2, sf::Color clr2)
{
    sf::RenderWindow wnd(sf::VideoMode(WND_W, WND_H), "Numeric Methods. Laboratory work 2.");

    sf::VertexArray graph2(sf::PrimitiveType::LineStrip, rng2.size());
    sf::VertexArray graph1(sf::PrimitiveType::LineStrip, rng1.size());

    sf::Font font;
    font.loadFromFile("arial.ttf");

    float q = 1/H_Max * (WND_W - 2 * WND_PAD);

    int cnt = 0;
    auto iter = rng2.begin();
    for (; iter != rng2.end(); iter++, cnt++)
    {
        graph2[cnt].position = sf::Vector2f(WND_PAD + q * (iter->x), WND_H - WND_PAD - q * (iter->y));
        graph2[cnt].color = clr2;
    }

    cnt = 0;
    iter = rng1.begin();
    for (; iter != rng1.end(); iter++, cnt++)
    {
        graph1[cnt].position = sf::Vector2f(WND_PAD + q * (iter->x), WND_H - WND_PAD - q * (iter->y));
        graph1[cnt].color = clr1;
    }

    sf::VertexArray axes(sf::PrimitiveType::LineStrip, 3);
    axes[0].position = sf::Vector2f (WND_W - WND_PAD,WND_H - WND_PAD);
    axes[0].color = sf::Color::Black;
    axes[1].position = sf::Vector2f (WND_PAD,WND_H - WND_PAD);
    axes[1].color = sf::Color::Black;
    axes[2].position = sf::Vector2f (WND_PAD,WND_PAD);
    axes[2].color = sf::Color::Black;

    wnd.clear(sf::Color::White);

    wnd.draw(graph2);
    wnd.draw(graph1);
    wnd.draw(axes);

    for (float f = 0; q * f <= WND_W - 2 * WND_PAD; f+=0.1f)
    {
        sf::String str(std::to_string(f));
        sf::Text txt(str, font, 15);
        txt.setFillColor(sf::Color::Black);
        txt.setPosition(WND_PAD + q * f, WND_H - 0.9f * WND_PAD);

        sf::VertexArray tmp(sf::PrimitiveType::LineStrip, 2);
        tmp[0].position = sf::Vector2f(WND_PAD + q * f, WND_H - WND_PAD);
        tmp[0].color = sf::Color::Black;
        tmp[1].position = sf::Vector2f(WND_PAD + q * f, WND_H - 0.9f * WND_PAD);
        tmp[1].color = sf::Color::Black;

        wnd.draw(tmp);
        wnd.draw(txt);
    }
    for (float f = 0; q * f <= WND_H - 2 * WND_PAD; f+=0.1f)
    {
        sf::String str(std::to_string(f));
        sf::Text txt(str, font, 15);
        txt.setFillColor(sf::Color::Black);
        txt.setPosition(0.3f * WND_PAD, WND_H - WND_PAD -  q * f);

        sf::VertexArray tmp(sf::PrimitiveType::LineStrip, 2);
        tmp[0].position = sf::Vector2f(WND_PAD, WND_H - WND_PAD - q * f);
        tmp[0].color = sf::Color::Black;
        tmp[1].position = sf::Vector2f(0.9f * WND_PAD, WND_H - WND_PAD - q * f);
        tmp[1].color = sf::Color::Black;

        wnd.draw(tmp);
        wnd.draw(txt);
    }

    wnd.display();

    while (wnd.isOpen())
    {
        sf::Event event{};
        while (wnd.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) wnd.close();
        }

        /*wnd.clear(sf::Color::White);

        wnd.draw(graph2);
        wnd.draw(graph1);

        wnd.display();*/
    }
}

int main()
{

    range ex = arange(Delta_exact, 0.03, H_Max, H);
    range calc = arange(Delta_calc, 0.03, H_Max, H_Max / K_Max);

    plot(ex, sf::Color::Red, calc, sf::Color::Blue);

    return 0;
}
