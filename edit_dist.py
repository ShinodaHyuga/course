def edit_distance(str1, str2, insert_cost, delete_cost, replace_cost):
    # 文字列の長さを取得
    m = len(str1)
    n = len(str2)

    # コストの表を初期化
    dp = [[0 for _ in range(n + 1)] for _ in range(m + 1)]

    # 初期化
    for i in range(m + 1):
        dp[i][0] = i * delete_cost
    for j in range(n + 1):
        dp[0][j] = j * insert_cost

    # 編集距離を計算
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if str1[i - 1] == str2[j - 1]:
                replace = 0
            else:
                replace = replace_cost
            dp[i][j] = min(
                dp[i - 1][j] + delete_cost,     # 削除
                dp[i][j - 1] + insert_cost,     # 挿入
                dp[i - 1][j - 1] + replace      # 置換
            )

    # コストの表を表示（オプション）
    for row in dp:
        print(row)

    # 編集距離を取得
    distance = dp[m][n]

    # 結果を返す
    return distance

# 2つの文字列とコストを入力
str1 = "ACATAC"
str2 = "AGGTCA"
insert_cost = 2
delete_cost = 2
replace_cost = 1

# 編集距離を計算
result = edit_distance(str1, str2, insert_cost, delete_cost, replace_cost)

# 結果を表示
print(f"編集距離: {result}")
