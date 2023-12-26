import numpy as np

def least_squares_fit(x, y):
    # データの数
    n = len(x)

    # Σxi, Σyi, Σxiyi, Σxi^2 の計算
    sum_x = np.sum(x)
    sum_y = np.sum(y)
    sum_xy = np.sum(x * y)
    sum_x_squared = np.sum(x**2)
    
    print(f"sum_x = {sum_x}")
    print(f"sum_y = {sum_y}")
    print(f"sum_xy = {sum_xy}")
    print(f"sum_x_squared = {sum_x_squared}")

    # 最小二乗法による傾き(a)と切片(b)の計算
    a = (n * sum_xy - sum_x * sum_y) / (n * sum_x_squared - sum_x**2)
    b = (sum_y - a * sum_x) / n

    return a, b

# 座標データの入力
x_values = np.array([4, 15, 30, 100])
y_values = np.array([-17, -4, -7, 50])

# 最小二乗法で傾きと切片を計算
slope, intercept = least_squares_fit(x_values, y_values)

# 結果の表示
print(f"傾き (a): {slope}")
print(f"切片 (b): {intercept}")
