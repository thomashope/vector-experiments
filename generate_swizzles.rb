#!ruby
components = %w[x y z]

# Generate all 2-letter combinations, excluding duplicates like xx, yy, zz
pairs = components.product(components).reject { |a, b| a == b }

# Write to types file
File.open("vec3_swizzle_types.inl", "w") do |types_file|
  pairs.each do |a, b|
    types_file.puts "swizzle_v3_v2_t(#{a},#{b})"
  end
end

# Write to members file
File.open("vec3_swizzles.inl", "w") do |swizzles_file|
  pairs.each do |a, b|
    swizzles_file.puts "swizzle_v3_v2_member(#{a},#{b})"
  end
end