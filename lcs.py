def longest_common_subsequence(str1, str2):
    m = len(str1)
    n = len(str2)

    # LCSの長さを格納するテーブルを初期化
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    # LCSを計算
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if str1[i - 1] == str2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])

    # LCSの長さを取得
    lcs_length = dp[m][n]

    # LCSを復元
    lcs = []
    i, j = m, n
    while i > 0 and j > 0:
        if str1[i - 1] == str2[j - 1]:
            lcs.append(str1[i - 1])
            i -= 1
            j -= 1
        elif dp[i - 1][j] > dp[i][j - 1]:
            i -= 1
        else:
            j -= 1

    # LCSを逆順にして文字列に変換
    lcs = ''.join(reversed(lcs))

    # LCSの表を表示
    for row in dp:
        print(row)

    return lcs, lcs_length

# 2つの文字列を入力
str1 = "LITTLE"
str2 = "ITALY"

# 最長共通部分列を計算
lcs, length = longest_common_subsequence(str1, str2)

# 結果を表示
print(f"最長共通部分列: {lcs}")
print(f"最長共通部分列の長さ: {length}")
