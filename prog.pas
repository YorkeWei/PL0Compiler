var x, y, z, q, r, n, f;

procedure multiply;
var a, b;
begin
  a := x;
  b := y;
  z := 0;
  while b > 0 do
  begin
    if odd b then z := z + a;
    a := 2 * a;
    b := b / 2
  end
end;

procedure divide;
var w;
begin
  r := x;
  q := 0;
  w := y;
  while w <= r do w := 2 * w;
  while w > y do
  begin
    q := 2 * q;
    w := w / 2;
    if w <= r then
    begin
      r := r - w;
      q := q + 1
   end
  end
end;

procedure gcd;
var w, g;
begin
  w := x;
  g := y;
  while w # g do
  begin
    if w < g then g := g - w;
    if g < w then w := w - g
  end;
  z := w
end;

procedure fact;
begin
  if n > 1 then
  begin
    f := n * f;
    n := n - 1;
    call fact
  end
end;

procedure staticlink;
var a,modifier;
procedure modifier;
begin
modifier:=1;
a := 5;
q := 3;
end;
begin
a:=3;
if n = 0 then
begin
call modifier
end;
if n > 0 then
begin
n := n - 1;
call staticlink;
end;
write(a);
end;

begin
  read(x,y); call multiply; write(z);
  read(x,y); call divide; write(q);write(r);
  read(x,y); call gcd; write(z);
  read(n); f := 1; call fact; write(f);
  n:=3;q:=2; call staticlink; write(n);write(q);
end