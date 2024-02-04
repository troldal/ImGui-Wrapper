//
// Created by kenne on 02/02/2024.
//

#pragma once

#include <imw.hpp>

#include <functional>

class Plot : public imw::PlotBase< Plot >
{
    using BASE = imw::PlotBase< Plot >;
    friend BASE;

    static constexpr auto num_points = 10'000;
    static constexpr auto x_min      = -100.0;
    static constexpr auto x_max      = 100.0;

    std::function< double(double) > m_cosFunction = [](double x) { return std::cos(x); };
    std::function< double(double) > m_sinFunction = [](double x) { return std::sin(x); };

    bool plotSin = false;
    bool plotCos = false;

public:
    using BASE::BASE;

    void setSinSelected(bool selected) { plotSin = selected; }
    void setCosSelected(bool selected) { plotCos = selected; }

private:
    void exec()
    {
        static const auto x_step = (std::abs(x_max) + std::abs(x_min)) / num_points;

        static auto xs = std::array< double, num_points > {};
        static auto ys = std::array< double, num_points > {};

        if (plotSin) {
            auto x = x_min;
            for (std::size_t i = 0; i < num_points; ++i) {
                xs[i] = x;
                ys[i] =m_sinFunction(x);
                x += x_step;
            }

            //ImPlot::SetNextLineStyle(IMPLOT_AUTO_COL, 10);
            const auto plot_label = fmt::format("##function{}", static_cast< int >(1));
            ImPlot::PlotLine(plot_label.data(), xs.data(), ys.data(), num_points, 0);
        }

        if (plotCos) {
            auto x = x_min;
            for (std::size_t i = 0; i < num_points; ++i) {
                xs[i] = x;
                ys[i] =m_cosFunction(x);
                x += x_step;
            }

            //ImPlot::SetNextLineStyle(IMPLOT_AUTO_COL, 10);
            const auto plot_label = fmt::format("##function{}", static_cast< int >(2));
            ImPlot::PlotLine(plot_label.data(), xs.data(), ys.data(), num_points, 0);
        }
    }
};