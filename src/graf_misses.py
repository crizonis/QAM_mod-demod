# minimum version Python 3.12
# script required installed library matplotlib

import sys
import matplotlib.pyplot as plt


def draw_graf(axes_x_: list, axes_y_: list, names_: list) -> None:
    line_types = ['o-g', 'o-r', 'o-y']
    plt.figure("plot QAM misses")
    for i_ in range(len(axes_x_)):
        graf = plt.plot(axes_x_[i_], axes_y_[i_], line_types[i_])
        plt.setp(graf, markersize=0.5)
    plt.legend(names_)
    axes = plt.gca()
    axes.set_facecolor('#111111')

    axes.spines['left'].set_color('black')
    axes.spines['bottom'].set_color('black')

    axes.tick_params(axis='x', colors='black')
    axes.tick_params(axis='y', colors='black')

    plt.grid(color='#884444', visible=True)
    plt.title("QAM misses \n(Dispersion - Number of misses)")

    plt.show()


if __name__ == "__main__":
    if 1 < len(sys.argv) < 5:
        axes_X = list()
        axes_Y = list()
        names = list()
        for i in range(0, len(sys.argv)-1):
            with open(str(sys.argv[i+1]), 'r') as file:
                axes_X.append(list())
                axes_Y.append(list())
                names.append(str(sys.argv[i+1]).lstrip('stat_misses_').rstrip('.csv'))
                while line := file.readline():
                    x, y = map(float, line.split(";"))
                    axes_X[i].append(x)
                    axes_Y[i].append(y)
        draw_graf(axes_X, axes_Y, names)
    else:
        print("incorrect number of parameters!")

