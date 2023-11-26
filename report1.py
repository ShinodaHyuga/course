import argparse

# コマンドライン引数をパースするための関数
def get_args():
    psr = argparse.ArgumentParser()
    psr.add_argument('-i', '--input', required=True)
    return psr.parse_args()

# ヘッダー部分を出力する関数
def print_header(input):
    # 入力された文字列を表示
    print(f"input: {input}")
    # スタックトレースのヘッダーを出力
    print("\t文字\t操作\tスタック")
    print("-" * 32)
    
# スタックトレースを行う関数
def execution_stack_trace(input_txt):
    input_arr = list(input_txt)
    open_brackets = ["(", "[", "{"]
    close_brackets = [")", "]", "}"]
    stack = list()
    
    for step in range(len(input_arr)):
        s = input_arr[step]
        if s in open_brackets:
            operation = "push"
            stack.append(s)
        elif s in close_brackets:
            open_brackets_num = open_brackets.index(stack[-1])
            close_brackets_num = close_brackets.index(s)
            # 括弧が閉じれるかの判定
            if open_brackets_num != close_brackets_num:
                break
            else:
                stack.pop()
                operation = "pop"
        else:
            operation = "pass"
            
        print_stack_trace(step + 1, s, operation, stack)
    return stack
        
# スタックトレースの結果を出力する関数
def print_stack_trace(step, s, operation, stack):
    step_str = f" {step}" if step < 10 else str(step)
    print(f"{step_str}:\t{s}\t{operation}\t{' '.join(map(str, stack))}")

# フッター部分を出力する関数
def print_footer(stack):
    ans = "OK" if len(stack) == 0 else "error"
    # スタックの状態と結果（OK または error）を表示
    print(f"len(stk) = {len(stack)} {ans}")
    
if __name__ == '__main__':
    input_txt = get_args().input
    print_header(input_txt)
    ans = execution_stack_trace(input_txt)
    print_footer(ans)

