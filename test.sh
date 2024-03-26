ex0="1,0,0"
out0=`./pa4 -e examples/invalidtree0.b`
diff <(echo $ex0) <(echo $out0)