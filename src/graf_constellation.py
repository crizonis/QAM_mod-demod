# minimum version Python 3.12
# script required installed library matplotlib

import sys
import matplotlib.pyplot as plt


def draw_graf(axis_x_: list, axis_y_: list) -> None:
    plt.figure("plot QAM constellation")
    graf = plt.plot(axis_x_, axis_y_, 'go')
    axes = plt.gca()
    axes.set_facecolor('#111111')

    axes.spines['right'].set_position('zero')
    axes.spines['right'].set_color('white')

    axes.spines['top'].set_position('zero')
    axes.spines['top'].set_color('white')

    axes.spines['left'].set_color('black')
    axes.spines['bottom'].set_color('black')

    axes.tick_params(axis='x', colors='black')
    axes.tick_params(axis='y', colors='black')

    ticks = [x/10 for x in range(-12, 13, 2)]
    plt.xticks(ticks)
    plt.yticks(ticks)
    plt.setp(graf, markersize=0.5)
    plt.grid(color='#884444', visible=True)
    plt.title("QAM constellation \n(I-Q)")

    plt.show()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("incorrect number of parameters!")
    else:
        axis_X = list()
        axis_Y = list()
        with open(str(sys.argv[1]), 'r') as file:
            while line := file.readline():
                x, y = map(float, line.split(";"))
                axis_X.append(x)
                axis_Y.append(y)
        draw_graf(axis_X, axis_Y)

