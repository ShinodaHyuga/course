import argparse
# コマンドライン引数をパースするための関数
def get_args():
    psr = argparse.ArgumentParser()
    psr.add_argument('-f', '--file', required=True)
    return psr.parse_args()
# この関数で階層レベルの配列を作成
# 引数：各⾏のスペース数の配列、返り値：各⾏のセクション番号の配列
def create_section_arr(space_num_arr):
    current_sec = [1] # 現在のセクション
    section_arr = [current_sec[:]] # 全体のセクション

    for i in range(1, len(space_num_arr)):
        # 1 つ前の⽂字列のスペース数との差をとる
        space_num_diff = space_num_arr[i - 1] - space_num_arr[i]

        if space_num_diff == 0:
            current_sec[-1] += 1
            # インデントが深くなる時の処理
        elif space_num_diff < 0:
            current_sec.append(1)
        # インデントが浅くなる時の処理
        else:
            # インデントの差分だけ pop()する
            for _ in range(space_num_diff):
                current_sec.pop()
            current_sec[-1] += 1

        section_arr.append(current_sec[:]) # 現在の section のコピーを追加
    return section_arr
# 出⼒⽤の関数
# 出⼒は ./data/hoge_result.txt に書き込む
def write_section(section_arr, line_arr, output_file):
    with open(output_file, mode="w") as f:
        for i in range(len(section_arr)):
            ind_section = ".".join([str(s) for s in section_arr[i]])
            f.write(f"{ind_section} {line_arr[i]}\n")
    print(f"Writing to file: {output_file}")

def main():
    input_file = get_args().file
    output_file = f"{input_file[:input_file.rfind('.')]}_result.txt"

    with open(input_file, mode="r") as f:
        lines = f.readlines()

    space_num_arr = [len(line) - len(line.strip()) for line in lines]
    line_arr = [line.strip() for line in lines]

    section_arr = create_section_arr(space_num_arr)
    write_section(section_arr, line_arr, output_file)

if __name__ == "__main__":
    main()
