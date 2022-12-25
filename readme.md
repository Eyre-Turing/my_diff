一个漏洞百出的文件对比工具

# 编译方法

```bash
g++ my_diff.cpp -lm -o my_diff
```

# 使用方法

```bash
./my_diff a.txt b.txt		# 比较（找不同）
./my_diff a.txt b.txt comm	# 找相同
```

