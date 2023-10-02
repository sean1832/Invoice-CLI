using Microsoft.Office.Interop.Excel;
using CommandLine;
using Range = Microsoft.Office.Interop.Excel.Range;
using Application = Microsoft.Office.Interop.Excel.Application;
using System.Runtime.InteropServices;
using System;
using System.IO;
using System.Reflection;
using ExcelWorker;

class Options
{
    private string _inPath;
    private string _outPath;

    [Option('c', "command", Required = true, HelpText = "Command to execute.")]
    public string Command { get; set; }

    [Option('p', "in-path", Required = true, HelpText = "Excel file path.")]
    public string InPath
    {
        get => _inPath;
        set => _inPath = $"{Directory.GetCurrentDirectory()}/{value}";
    }

    [Option('o', "out-path", Required = true, HelpText = "Output file path.")]
    public string OutPath
    {
        get => _outPath;
        set => _outPath = $"{Directory.GetCurrentDirectory()}/{value}";
    }

    [Option('w', "worksheet", Required = true, HelpText = "Worksheet number.")]
    public int Worksheet { get; set; }
}

namespace excel_worker
{
    internal class Worker
    {
        static int Main(string[] args)
        {
            ProjectInfo projectInfo = new ProjectInfo(Assembly.GetExecutingAssembly());

            ConsoleMsg.InfoMessage($"Executing {projectInfo.Title}.exe...");
            Console.WriteLine();
            Console.WriteLine($"{projectInfo.Title} v{projectInfo.Version}");
            Console.WriteLine($"{projectInfo.Description}");
            Console.WriteLine($"{projectInfo.Copyright}");
            Console.WriteLine();

            try
            {
                // parse command line arguments
                Parser.Default.ParseArguments<Options>(args)
                    .WithParsed<Options>(o =>
                    {
                        switch (o.Command)
                        {
                            case "write":
                                Console.WriteLine("Not yet implemented.");
                                //WriteExcel(o.InPath, o.Worksheet, 2, "2020-01-01", 1, 1, "test", 1, "test");
                                break;
                            case "pdf":
                                PdfExport(o.InPath, o.Worksheet, o.OutPath);
                                break;
                            default:
                                throw new ApplicationException($"[ExcelWorker.Main] invalid command {o.Command}.");
                        }
                    });
                return 0;
            }
            catch (Exception e)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"[ExcelWorker.Main] ERROR -> {e.Message}");
                return 1;
                // bug: handle excel closure
            }
        }

        private static void PdfExport(string path, int worksheetNum, string outPath)
        {
            // Check if file exists
            if (!File.Exists(path))
            {
                throw new ApplicationException($"[ExcelWorker.PdfExport] file {path} does not exist.");
            }

            if (worksheetNum < 1)
            {
                throw new ApplicationException($"[ExcelWorker.PdfExport] Invalid worksheet number {worksheetNum}. Must be 1 or greater.");
            }


            string folder = Path.GetDirectoryName(outPath);
            if (!Directory.Exists(folder) && folder != "")
            {
                Directory.CreateDirectory(folder); // Create directory if it does not exist
            }

            Microsoft.Office.Interop.Excel.Application excel = null;
            Workbook wb = null;
            try
            {
                excel = new Microsoft.Office.Interop.Excel.Application();
                //excel.Visible = true; // make Excel visible
                wb = excel.Workbooks.Open(path);
                Worksheet ws = (Worksheet)wb.Worksheets[worksheetNum];

                // Adjust the PageSetup properties
                ws.PageSetup.FitToPagesWide = 1;
                ws.PageSetup.FitToPagesTall = false;
                ws.PageSetup.Zoom = false; // Set to false to override other settings


                // export as pdf
                ws.ExportAsFixedFormat(XlFixedFormatType.xlTypePDF, outPath);

                Console.WriteLine($"[excel-worker.PdfExport] pdf file exported at {outPath}");
            }
            catch (Exception ex)
            {
                throw new ApplicationException($"[excel-worker.PdfExport] ERROR -> {ex.Message}");
            }
            finally
            {
                if (wb != null)
                {
                    try
                    {
                        wb.Close(false);
                        Marshal.ReleaseComObject(wb);
                    }
                    catch (Exception e)
                    {
                        Console.ForegroundColor = ConsoleColor.Yellow;
                        Console.WriteLine($"[excel-worker.PdfExport] WARNING -> Could not close the workbook: {e.Message}");
                        Console.ResetColor();
                    }
                }

                if (excel != null)
                {
                    try
                    {
                        excel.Quit();
                        Marshal.ReleaseComObject(excel);
                    }
                    catch (Exception e)
                    {
                        Console.ForegroundColor = ConsoleColor.Yellow;
                        Console.WriteLine($"[excel-worker.PdfExport] WARNING -> Could not quit Excel: {e.Message}");
                        Console.ResetColor();
                    }
                }

                GC.Collect();
                GC.WaitForPendingFinalizers();
            }
        }

    }
}

