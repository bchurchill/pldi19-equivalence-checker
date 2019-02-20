#!/usr/bin/env ruby

require 'set'

@data = {}

@blacklist = { :gcc => { "s351" => true }, :llvm => {} }

@debug = false
if ARGV.size > 0 and ARGV[0] == "--debug"
  puts "DEBUG MODE"
  @debug = true
end


def collect_data(compiler1, compiler2, benchmark)
  puts "Collecting data #{compiler1} #{compiler2} #{benchmark}"

  prefix = "#{benchmark}_#{compiler1}_#{compiler2}"
  num = 0

  while File.exist?("traces/#{prefix}.#{num}")
    name = "traces/#{prefix}.#{num}"
    num = num+1
  end

  if name.nil?
    puts "File not found!"
    return
  else
    puts "Found #{name}"
  end

  total_edges = 0
  total_nodes = 0
  current_invs = 0
  total_invs = 0
  counting = false

  File.open(name).each do |line|
    line = line.strip

    if m = /PROOF COMPLETE/.match(line) then
      counting = true
      current_invs = 0
      next
    end

    if counting and m = /STATE/.match(line) then
      total_nodes = total_nodes + 1
      total_invs = current_invs + total_invs
      current_invs = 0
      next
    end

    if counting and m = /.*to \(/.match(line) then
      total_edges = total_edges + 1
      next
    end

    if /FAIL/.match(line) then
      next
    end

    current_invs = current_invs + 1

  end

  puts "       total edges: #{total_edges}"
  puts "       total nodes: #{total_nodes}"
  puts "       conjuncts  : #{total_invs}"

  @data[benchmark][compiler1][compiler2] = {
    :total_edges => total_edges,
    :total_nodes => total_nodes,
    :total_invs  => total_invs
  }
end

def avg(list)
  sum = 0
  list.each { |x| sum += x }
  sum = sum / list.length
  sum
end

def min(list)
  list.min
end

def max(list)
  list.max
end

def median(array)
  sorted = array.sort
  len = sorted.length
  (sorted[(len - 1) / 2] + sorted[len / 2]) / 2.0
end

def output_data(benchmarks)
  filename = "edges.out"

  nodes = []
  edges = []
  invariants = []

  File.open(filename, 'w') do |file|
    file.write("benchmark total_edges\n")
    @data.each do |benchmark,m2|
      m2.each do |compiler1,m3|
        m3.each do |compiler2,m4| 
          file.write("#{benchmark}-#{compiler2} #{m4[:total_edges]} #{m4[:total_nodes]}\n")
          if m4[:total_edges] > 0 then
            edges.push(m4[:total_edges])
            nodes.push(m4[:total_nodes])
            if m4[:total_invs] > 20 then 
              invariants.push(m4[:total_invs])
            end
          end
        end
      end
    end
  end

  puts "Nodes average #{avg(nodes)} min #{min(nodes)} max #{max(nodes)} median #{median(nodes)}"
  puts "Edges average #{avg(edges)} min #{min(edges)} max #{max(edges)} median #{median(edges)}"
  puts "Invs average #{avg(invariants)} min #{min(invariants)} max #{max(invariants)} median #{median(invariants)}"

end

def main
  benchmarks = []
  i = 0
  File.open('benchmarks').each do |line|
    benchmarks.push(line.strip)
    i = i + 1
    break if i > 3 and @debug
  end

  benchmarks.each do |benchmark|
    @data[benchmark] = {}
    @data[benchmark][:baseline] = {}

    if not @blacklist[:gcc][benchmark] then
      @data[benchmark][:baseline][:gcc] = {}
      collect_data(:baseline, :gcc, benchmark)
    end

    if not @blacklist[:llvm][benchmark] then
      @data[benchmark][:baseline][:llvm] = {}
      collect_data(:baseline, :llvm, benchmark)
    end
  end

  output_data(benchmarks)
end

main

