import sys
f = open(sys.argv[1], "rb")
count = {}
while True:
	c = f.read(1)
	if len(c) == 0: break
	c = "{:02x}".format(ord(c))
	if count.has_key(c):
		count[c] += 1
	else:
		count[c] = 1
for key in count:
	print key, count[key]
