import math
from tabulate import tabulate
import matplotlib.pyplot as plt
import numpy as np


# time_on_air = t_payload + t_preamble = (N_payload + N_preamble) * symbol_time
def toA_with_size(sf, PL):
    bw = 125
    IH = 0
    DE = 0
    if sf >= 11:
        DE = 1
    CR = 1
    symbol_time = (pow(2, sf) / bw)
    t_preamble = (4.25 + 8) * symbol_time
    n_payload = 8 + max(math.ceil((8*PL - 4*sf + 28 + 16 - 20*IH) / (4*(sf - 2*DE))) * (CR + 4), 0)
    t_payload = n_payload * symbol_time
    return round(t_payload + t_preamble, 2)


def display_table(sf, payload_size):
    table = []
    table.append(["SF", "16B", "32B", "51B"])
    for _sf in sf:
        data = []
        data.append(_sf)
        for pl in payload_size:
            data.append(str(toA_with_size(_sf, pl)) + "ms")
        table.append(data)
    print(tabulate(table, headers='firstrow', tablefmt='fancy_grid'))


def display_plot(sf, payload_size):
    for _sf in sf:
        y_points = []
        x_points = []
        for pl in payload_size:
            x_points.append(pl)
            toa = toA_with_size(_sf, pl)
            print(f'sf {_sf}, pl = {pl}, toa = {toa}')
            y_points.append(toA_with_size(_sf, pl))
        plt.plot(x_points, y_points, 'o:r')
        plt.title(f'SF = {_sf}')
        plt.xlabel("payload length")
        plt.ylabel("time on air")
        plt.show()


def main():
    sf = [x for x in range(7, 13)]
    payload_size = [16, 32, 51]
    display_table(sf, payload_size)
    display_plot(sf, payload_size)


main()