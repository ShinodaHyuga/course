TP = 9 / 1000
FP = 9 / 1000
FN = 1 / 1000
TN = 981 / 1000

def calc_accuracy(TP, FP, FN, TN):
    return (TP + TN) / (TP + FN + FP + TN)

def calc_precision(TP, FP):
    return TP / (TP + FP)

def calc_recall(TP, FN):
    return TP / (TP + FN)

# 精度 (accuracy)
accuracy = calc_accuracy(TP, FP, FN, TN)
print(f"accuracy: {accuracy}")

# 適合率 (precision)
precision = calc_precision(TP, FP)
print(f"precision: {precision}")

# 再現率 (recall)
recall = calc_recall(TP, FN)
print(f"recall: {recall}")
