components = %w[x y z]

# Generate all 2-component combinations
pairs = components.product(components)
triples = components.product(components, components)

File.open("vec3_swizzles.inl", "w") do |members_file|
  pairs.each do |a, b|
    if a != b
      members_file.puts "swizzle_v3_v2(#{a},#{b})"
    end
  end

  triples.each do |a, b, c|
    if a != b && a != c && b != c
      members_file.puts "swizzle_v3_v3(#{a},#{b},#{c})"
    end
  end


  pairs.each do |a, b|
    if a == b
      members_file.puts "splat_v3_v2(#{a},#{b})"
    end
  end

  triples.each do |a, b, c|
    if a == b || a == c || b == c
      members_file.puts "splat_v3_v3(#{a},#{b},#{c})"
    end
  end
end