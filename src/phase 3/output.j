.source input.txt
.class public test
.super java/lang/Object

.method public <init>()V
aload_0
invokenonvirtual java/lang/Object/<init>()V
return
.end method

.method public static main([Ljava/lang/String;)V
.limit locals 100
.limit stack 100
iconst_0
istore 1
fconst_0
fstore 2
.line 1
iconst_0
istore 3
.line 2
L_0:
ldc 50
istore 3
.line 3
L_1:
iload 3
ldc 50
if_icmpeq L_2
goto L_3
.line 4
L_2:
.line 5
ldc 200
istore 3
.line 6
goto L_4
.line 7
.line 8
L_3:
.line 9
ldc 20
istore 3
.line 10
.line 11
L_4:
iload 3
istore 1
getstatic      java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/println(I)V
.line 12
L_5:
iconst_0
istore 4
.line 13
L_6:
ldc 0
istore 4
L_7:
iload 4
ldc 10
if_icmplt L_9
goto L_10
L_8:
iload 4
ldc 1
iadd
istore 4
goto L_7
L_9:
.line 14
iload 4
istore 1
getstatic      java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/println(I)V
.line 15
goto L_8
.line 16
L_10:
L_11:
iload 4
ldc 0
if_icmpge L_12
goto L_14
L_12:
.line 17
iload 4
istore 1
getstatic      java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/println(I)V
.line 18
L_13:
iload 4
ldc 1
isub
istore 4
.line 19
goto L_11
L_14:
return
.end method
