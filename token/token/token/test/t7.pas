var k:integer;
procedure TestCase09(var k:integer);
var a,b:integer;
e, f:array [10] of integer;
begin
	a:= 1;
	b:= 2;
	e[a]:=1;
	f[b]:=2;
	if b<=e[a] then write(12) else write(13);
	if e[a]<>a then write(13) else write(14);

end;
begin 
	TestCase09(k)
end.

{2368111314}
