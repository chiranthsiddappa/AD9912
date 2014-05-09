min = "float dBMMinVals[391] = {"
max = "float dBMMaxVals[391] = {"

ampDataHashArr = []
File.open("AmplitudeCalibrationData.csv").each do |line|
  lineSplit = line.split(",")
  ampDataHashArr.push({"frequency" => lineSplit[0].to_i, "min" => lineSplit[1].to_f, "max" => lineSplit[2].to_f})
end

ampDataHashArr.each do |dat|
  if(dat["frequency"] >= 60)
    min += "#{dat["min"]},"
    max += "#{dat["max"]},"
  end
end

min += "};"
max += "};"

puts min
puts max
